package Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk;
use strict;
use warnings;
use Tie::Hash::MinPerfHashTwoLevel::Mount;
use Tie::Hash::MinPerfHashTwoLevel::OnDisk ':flags', ':xs_subs';
our $VERSION = '0.16';
our $DEFAULT_VARIANT = 6;
our @ISA= ("Tie::Hash::MinPerfHashTwoLevel::OnDisk");

# this also installs the XS routines we use into our namespace.
use Exporter qw(import);
use Carp;
use constant {
    DEBUG               => 0,
    MOUNT_IDX           => 0,
    SEPARATOR_IDX       => 1,
};

our %EXPORT_TAGS = (
    'all' => [],
    'flags' => [],
    'magic' => [],
);

my $scalar_has_slash= scalar(%EXPORT_TAGS)=~m!/!;
our @EXPORT_OK = ( @{ $EXPORT_TAGS{'all'} } );

our @EXPORT = qw();

sub mph2l_multi_tied_hashref {
    my ($file, %opts)= @_;
    tie my %tied, __PACKAGE__, $file, %opts;
    return \%tied;
}

sub _debug {
    my $self= shift;
    my ($sub)= (caller(1))[3];
    $sub=~s/^(\w+::)+//;
    my @extra;
    while (@_) {
        my ($key,$value)= splice @_,0,2;
        push @extra, sprintf "%s=%s", $key, defined $value ? "'$value'" : "undef";
    }
    my @v= ($sub, 0+$self, "'$self->{prefix}'", $self->{file}, $self->{leftmost_idx}, $self->{iter_idx}, $self->{rightmost_idx},
        join " ",@extra);
    !defined($v[$_]) and die "undefined argument $_" for 0..$#v;
    warn sprintf "%-8s %08x prefix=%-10s file='%s' leftmost_idx=%d iter_idx=%d rightmost_idx=%d %s\n", @v;
}

sub new {
    my $self;
    if (@_==2) {
        $self= bless \my %opts, ref($_[0]);
        $opts{prefix}= $_[1][1];

        if (utf8::is_utf8($_[1][1])) { $opts{prefix_utf8}= $_[1][1]; }
        else { $opts{prefix_latin1}= $_[1][1]; };

        $opts{leftmost_idx}= $_[1][2];
        $opts{rightmost_idx}= $_[1][3];

        $opts{mount}= $_[0]->{mount};
        $opts{level}= $_[0]->{level} + 1;
        $opts{levels}= $_[0]->{levels};
    } else {
        my ($class,%opts)= @_;
        my $sep= $opts{separator} //= "/";
        if (length($sep) != 1 or ord($sep) > 127) {
            die "Separator MUST be a single ASCII character (eg 0-127)";
        }
        my $mount= $opts{mount}= Tie::Hash::MinPerfHashTwoLevel::Mount->new(\%opts);
        $self= bless \%opts, $class;
        if ($self->get_hdr_variant != 6) {
            die "Cannot use ".__PACKAGE__." with a variant " . $self->get_hdr_variant . " file";
        }
        $self->{level}= 1;
        $self->{levels}||= 0;
        $self->{prefix} //= "";
        if (length(my $prefix= $self->{prefix})) {
            $self->{leftmost_idx} //= find_first_prefix($mount->[MOUNT_IDX],$prefix);
            if ( $self->{leftmost_idx} < 0 ) {
                $self->{rightmost_idx}= -1;
            } else {
                $self->{rightmost_idx} //= find_last_prefix($mount->[MOUNT_IDX],$prefix,$self->{leftmost_idx});
            }
        } else {
            $self->{leftmost_idx}= 0;
            $self->{rightmost_idx}= $self->get_hdr_num_buckets - 1;
        }
    }
    my $fetch_key_only=  $self->{fetch_key_only}= $self->{levels} && $self->{level} == $self->{levels};
    $self->{fetch_key_first}= $fetch_key_only ? 2 : (!$self->{prefix} || $self->{level} >= $self->{levels});
    DEBUG and $self->_debug();
    return $self;
}


*TIEHASH= *new;


# mixed XS/pure-perl version of FETCH - the real FETCH is in MinPerfHashTwoLevel.xs
sub _FETCH {
    my ($self, $key)= @_;
    DEBUG and $self->_debug(key=>$key);
    my ($value, $leftmost_idx, $rightmost_idx);
    my $mount= $self->{mount};
    $key= $self->{prefix} . $key;
    if ($self->{fetch_key_first}) {
        if (fetch_by_key($mount->[MOUNT_IDX], $key, $value)) {
            DEBUG and $self->_debug(key=>$key,value=>$value);
            return $value;
        } elsif ($self->{fetch_key_only}) {
            return undef;
        }
    }

    $key .= $mount->[SEPARATOR_IDX];
    $leftmost_idx= find_first_last_prefix($mount->[MOUNT_IDX], $key, $rightmost_idx,
                        $self->{leftmost_idx}, $self->{rightmost_idx});
    if ($leftmost_idx >= 0) {
        my %hash;
        tie %hash, $self, [ $self, $key, $leftmost_idx, $rightmost_idx ];
        $value= \%hash;
    }
    return $value;
}

# mixed XS/pure-perl version of FETCH - the real FETCH is in MinPerfHashTwoLevel.xs
sub _NEXTKEY {
    my ($self)= @_;
    $self->{iter_idx}= $self->{leftmost_idx} unless defined $_[1];
    DEBUG and $self->_debug(lastkey=>$_[1]);
    my $mount= $self->{mount};

    fetch_by_index($mount->[MOUNT_IDX], $self->{iter_idx}, my $key)
        or return undef;

    my $l= length($self->{prefix});
    if (substr($key,0,$l) ne $self->{prefix}) {
        return undef;
    }

    my $ofs= index($key,$mount->[SEPARATOR_IDX],$l);
    my $part;
    if ($ofs<0) {
        $self->{iter_idx}++;
        $part= substr($key,$l);
    } else {
        $part= substr($key,$l,$ofs-$l);
        $self->{iter_idx}= find_last_prefix(
            $mount->[MOUNT_IDX],
            substr($key,0,$ofs+1), # include separator!
            $self->{iter_idx},
            $self->{rightmost_idx},
        ) + 1;
    }
    return $part;
}


sub SCALAR {
    my ($self)= @_;
    return $self->{scalar_buckets} //= do {
        my $bucket_count= $self->{rightmost_idx} - $self->{leftmost_idx} + 1;
        if ($bucket_count && $scalar_has_slash) {
            $bucket_count .= "/" . $bucket_count;
        }
        $bucket_count;
    };
}

sub UNTIE {
    my ($self)= @_;
}

sub STORE {
    my ($self, $key, $value)= @_;
    confess __PACKAGE__ . " is readonly, STORE operations are not supported";
}

sub DELETE {
    my ($self, $key)= @_;
    confess __PACKAGE__ . " is readonly, DELETE operations are not supported";
}

sub CLEAR {
    my ($self)= @_;
    confess __PACKAGE__ . " is readonly, CLEAR operations are not supported";
}

1;
__END__

=head1 NAME

Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk - construct or tie a "two level" minimal perfect hash based on disk

=head1 SYNOPSIS

  use Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk;

  Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk->make_file(
    file => $some_file,
    source_hash => $some_hash,
    comment => "this is a comment",
    debug => 0,
  );

  my %hash;
  tie %hash, "Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk", file => $some_file, separator=>"/";

=head1 DESCRIPTION

This module allows one to either construct, or use a precomputed minimal
perfect hash on disk via tied interface. The disk image of the hash is
loaded by using mmap, which means that multiple processes may use the
same disk image at the same time without memory duplication. The hash
is readonly, and may only contain string and undef values.

This is a variant of Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk which supports
using a single large hash to represent a HoHoH(oH?) using composite keys joined with
a designated separator.

=head2 METHODS

=over 4

=item make_file

Construct a new file from a given 'source_hash' argument. Takes the following arguments:

=over 4

=item file

The file name to produce, mandatory.

=item comment

An arbitrary piece of text of your choosing. Can be extracted from
the file later if needed. Only practical restriction on the value is
that it cannot contain a null.

=item seed

A 16 byte string (or a reference to one) to use as the seed for
the hashing and generation process. If this is omitted a standard default
is chosen.

If it should prove impossible to construct a solution using the seed chosen
then a new one will be constructed deterministically from the old until a
solution is found (see L<max_tries>) (prior to version v0.10 this used rand()).
Should you wish to access the seed actually used for the final solution
then you can pass in a reference to a scalar containing your chosen seed.
The reference scalar will be updated after successful construction.

Thus both of the following are valid:

    Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk->make_file(seed => "1234567812345678", ...);
    Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk->make_file(seed => \my $seed= "1234567812345678", ...);

=item no_dedupe

Speed up construction at the cost of a larger string buffer by disabling
deduplication of values and keys.  Same as setting the MPH_F_NO_DEDUPE bit
in compute_flags.

=item filter_undef

Ignore keys with undef values during construction. This means that exists() checks
may differ between source and the constructed hash table, but avoids the need to
store such keys in the resulting file, saving space. Same as setting the
MPH_F_FILTER_UNDEF bit in compute_flags.

=item max_tries

The maximum number of attempts to make to find a solution for this keyset.
Defaults to 3. 

=item debug

Enable debug during generation.

=item variant

The variant number for this type of file is 6.

=back

=item validate_file

Validate the file specified by the 'file' argument. Returns a list of
two values, 'variant' and 'message'. If the file fails validation the 'variant'
will be undef and the 'message' will contain an error message. If the file
passes validation the 'variant' will specify the variant of the file
(currently only 0 is valid), and 'message' will contain some basic information
about the file, such as how many keys it contains, the comment it was
created with, etc.

=back

=head2 TIED INTERFACE

  my %hash;
  tie %hash, "Tie::Hash::MinPerfHashTwoLevel::MultiLevelOnDisk",
    file => $some_file, flags => $flags;

will setup %hash to read from the mmapped image on disk as created by make_file().
The underlying image is never altered, and copies of the keys and values are made
when necessary. The flags field is an integer which contains bit-flags to control
the reading process, currently only one flag is supported MPH_F_VALIDATE which enables
a full file checksum before returning (forcing the data to be loaded and then read).
By default this validation is disabled, however basic checks of that the header is
sane will be performed on loading (or "mounting") the image. The tie operation may
die if the file is not found or any of these checks fail.

=head2 FILE FORMAT

The file format for variant 6/MultiLevelOnDisk is currently undocumented.

=head2 EXPORT

None by default.

=head1 SEE ALSO

Algorithm::MinPerfHashTwoLevel

=head1 AUTHOR

Yves Orton

=head1 COPYRIGHT AND LICENSE

Copyright (C) 2019 by Yves Orton

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself, either Perl version 5.18.4 or,
at your option, any later version of Perl 5 you may have available.

=cut
