#ifndef MPH_SEED_BYTES
#define MPH_SEED_BYTES (sizeof(U64) * 2)
#endif
#ifndef MPH_STATE_BYTES
#define MPH_STATE_BYTES (sizeof(U64) * 4)
#endif

#define U64 U64TYPE

#ifndef MPH_MAP_POPULATE
#ifdef MAP_POPULATE
#define MPH_MAP_POPULATE MAP_POPULATE
#else
#define MPH_MAP_POPULATE MAP_PREFAULT_READ
#endif
#endif

#define MPH_KEYSV_IDX               0
#define MPH_KEYSV_H1_KEYS           1
#define MPH_KEYSV_XOR_VAL           2
#define MPH_KEYSV_H0                3
#define MPH_KEYSV_KEY               4
#define MPH_KEYSV_KEY_NORMALIZED    5
#define MPH_KEYSV_KEY_IS_UTF8       6
#define MPH_KEYSV_VAL               7
#define MPH_KEYSV_VAL_NORMALIZED    8
#define MPH_KEYSV_VAL_IS_UTF8       9
#define MPH_KEYSV_SORT_INDEX        10

#define MPH_KEYSV_VARIANT           11
#define MPH_KEYSV_COMPUTE_FLAGS     12
#define MPH_KEYSV_STATE             13
#define MPH_KEYSV_SOURCE_HASH       14
#define MPH_KEYSV_BUF_LENGTH        15
#define MPH_KEYSV_BUCKETS           16
#define MPH_KEYSV_MOUNT             17
#define MPH_KEYSV_ITER_IDX          18
#define MPH_KEYSV_LEFTMOST_IDX      19
#define MPH_KEYSV_RIGHTMOST_IDX     20
#define MPH_KEYSV_PREFIX            21
#define MPH_KEYSV_PREFIX_UTF8       22
#define MPH_KEYSV_PREFIX_LATIN1     23
#define MPH_KEYSV_FETCH_KEY_FIRST   24
#define MPH_KEYSV_LEVEL             25
#define MPH_KEYSV_LEVELS            26
#define MPH_KEYSV_FAST_PROPS        27
#define MPH_KEYSV_KEYS_AV           28
#define MPH_KEYSV_SEPARATOR         29

#define COUNT_MPH_KEYSV 30

#define MOUNT_ARRAY_MOUNT_IDX 0
#define MOUNT_ARRAY_SEPARATOR_IDX 1

#define MPH_INIT_KEYSV(idx, str) STMT_START {                           \
    MY_CXT.keyname_sv[idx].sv = newSVpvn((str ""), (sizeof(str) - 1));       \
    PERL_HASH(MY_CXT.keyname_sv[idx].hash, (str ""), (sizeof(str) - 1));     \
} STMT_END

#define MPH_INIT_ALL_KEYSV() STMT_START {\
    MY_CXT_INIT;                                                \
    MPH_INIT_KEYSV(MPH_KEYSV_IDX,"idx");                        \
    MPH_INIT_KEYSV(MPH_KEYSV_H1_KEYS,"h1_keys");                \
    MPH_INIT_KEYSV(MPH_KEYSV_XOR_VAL,"xor_val");                \
    MPH_INIT_KEYSV(MPH_KEYSV_H0,"h0");                          \
    MPH_INIT_KEYSV(MPH_KEYSV_KEY,"key");                        \
    MPH_INIT_KEYSV(MPH_KEYSV_KEY_NORMALIZED,"key_normalized");  \
    MPH_INIT_KEYSV(MPH_KEYSV_KEY_IS_UTF8,"key_is_utf8");        \
    MPH_INIT_KEYSV(MPH_KEYSV_VAL,"val");                        \
    MPH_INIT_KEYSV(MPH_KEYSV_VAL_NORMALIZED,"val_normalized");  \
    MPH_INIT_KEYSV(MPH_KEYSV_VAL_IS_UTF8,"val_is_utf8");        \
    MPH_INIT_KEYSV(MPH_KEYSV_SORT_INDEX,"sort_index");          \
                                                                \
    MPH_INIT_KEYSV(MPH_KEYSV_VARIANT,"variant");                \
    MPH_INIT_KEYSV(MPH_KEYSV_COMPUTE_FLAGS,"compute_flags");    \
    MPH_INIT_KEYSV(MPH_KEYSV_STATE,"state");                    \
    MPH_INIT_KEYSV(MPH_KEYSV_SOURCE_HASH,"source_hash");        \
    MPH_INIT_KEYSV(MPH_KEYSV_BUF_LENGTH,"buf_length");          \
    MPH_INIT_KEYSV(MPH_KEYSV_BUCKETS,"buckets");                \
    MPH_INIT_KEYSV(MPH_KEYSV_MOUNT,"mount");                    \
    MPH_INIT_KEYSV(MPH_KEYSV_ITER_IDX,"iter_idx");              \
    MPH_INIT_KEYSV(MPH_KEYSV_LEFTMOST_IDX,"leftmost_idx");      \
    MPH_INIT_KEYSV(MPH_KEYSV_RIGHTMOST_IDX,"rightmost_idx");    \
    MPH_INIT_KEYSV(MPH_KEYSV_PREFIX,"prefix");                  \
    MPH_INIT_KEYSV(MPH_KEYSV_PREFIX_UTF8,"prefix_utf8");        \
    MPH_INIT_KEYSV(MPH_KEYSV_PREFIX_LATIN1,"prefix_latin1");    \
    MPH_INIT_KEYSV(MPH_KEYSV_FETCH_KEY_FIRST,"fetch_key_first");\
    MPH_INIT_KEYSV(MPH_KEYSV_LEVEL,"level");                    \
    MPH_INIT_KEYSV(MPH_KEYSV_LEVELS,"levels");                  \
    MPH_INIT_KEYSV(MPH_KEYSV_FAST_PROPS,"_fast_props");         \
    MPH_INIT_KEYSV(MPH_KEYSV_KEYS_AV,"_keys_av");               \
    MPH_INIT_KEYSV(MPH_KEYSV_SEPARATOR,"separator");            \
} STMT_END

#define MPH_F_FILTER_UNDEF          (1<<0)
#define MPH_F_DETERMINISTIC         (1<<1)
#define MPH_F_NO_DEDUPE             (1<<2)
#define MPH_F_VALIDATE              (1<<3)
#define MPH_F_HASH_ONLY             (1<<4)

#define MPH_F_IS_KEY                (1<<17)
#define MPH_F_IS_VAL                (1<<18)
#define MPH_F_COMPRESS_KEYS         (1<<19)
#define MPH_F_COMPRESS_VALS         (1<<20)

#define MPH_F_DEBUG                 (1<<30)
#define MPH_F_DEBUG_MORE            (1<<31)


#if 1
#define MPH_HASH_FOR_FETCH 0
#else
#define MPH_HASH_FOR_FETCH 1
#endif


#define MPH_MOUNT_ERROR_OPEN_FAILED     (-1)
#define MPH_MOUNT_ERROR_FSTAT_FAILED    (-2)
#define MPH_MOUNT_ERROR_TOO_SMALL       (-3)
#define MPH_MOUNT_ERROR_BAD_SIZE        (-4)
#define MPH_MOUNT_ERROR_MAP_FAILED      (-5)
#define MPH_MOUNT_ERROR_BAD_MAGIC       (-6)
#define MPH_MOUNT_ERROR_BAD_VERSION     (-7)
#define MPH_MOUNT_ERROR_BAD_OFFSETS     (-8)
#define MPH_MOUNT_ERROR_CORRUPT_TABLE   (-9)
#define MPH_MOUNT_ERROR_CORRUPT_STR_BUF (-10)
#define MPH_MOUNT_ERROR_CORRUPT_FILE    (-11)

#define MAGIC_DECIMAL 1278363728 /* PH2L */
#define MAGIC_BIG_ENDIAN_DECIMAL 1346908748

#define MPH_VALS_ARE_SAME_UTF8NESS_FLAG_BIT   1           /* 0000 0001 */
#define MPH_VALS_ARE_SAME_UTF8NESS_MASK       3           /* 0000 0011 */
#define MPH_VALS_ARE_SAME_UTF8NESS_SHIFT      1           /*  000 0001 */
#define MPH_KEYS_ARE_SAME_UTF8NESS_FLAG_BIT   4           /* 0000 0100 */
#define MPH_KEYS_ARE_SAME_UTF8NESS_MASK       (7 << 2)    /* 0001 1100 */
#define MPH_KEYS_ARE_SAME_UTF8NESS_SHIFT      3           /*    0 0011 */

/*
#ifndef av_top_index
#define av_top_index(x) av_len(x)
#endif
*/

#ifndef MPH_STATIC_INLINE
#ifdef PERL_STATIC_INLINE
#define MPH_STATIC_INLINE PERL_STATIC_INLINE
#else
#define MPH_STATIC_INLINE static inline
#endif
#endif

#define hv_fetch_ent_with_keysv(hv,keysv_idx,lval)                      \
    hv_fetch_ent(hv,keyname_sv[keysv_idx].sv,lval,keyname_sv[keysv_idx].hash)

#define hv_fetch_sv_with_keysv(sv,hv,keysv_idx,lval) STMT_START {               \
    HE *got_he= hv_fetch_ent_with_keysv(hv,keysv_idx,lval);                     \
    if (got_he) {                                                               \
        sv= HeVAL(got_he);                                                      \
    } else {                                                                    \
        if (lval) croak("failed to fetch item in hv_fetch_sv_with_keysv()");    \
        sv= NULL;                                                               \
    }                                                                           \
} STMT_END

#define hv_delete_ent_with_keysv(hv,keysv_idx) \
    hv_delete_ent(hv,keyname_sv[keysv_idx].sv,0,keyname_sv[keysv_idx].hash)


#define hv_store_ent_with_keysv(hv,keysv_idx,val_sv)                    \
    hv_store_ent(hv,keyname_sv[keysv_idx].sv,val_sv,keyname_sv[keysv_idx].hash)

#define hv_copy_with_keysv(hv1,hv2,keysv_idx) STMT_START {              \
    HE *got_he= hv_fetch_ent_with_keysv(hv1,keysv_idx,0);               \
    if (got_he) {                                                       \
        SV *got_sv= HeVAL(got_he);                                      \
        hv_store_ent_with_keysv(hv2,keysv_idx,got_sv);                  \
        SvREFCNT_inc(got_sv);                                           \
    }                                                                   \
} STMT_END



#define hv_setuv_with_keysv(hv,keysv_idx,uv)                            \
STMT_START {                                                            \
    HE *got_he= hv_fetch_ent_with_keysv(hv,keysv_idx,1);                \
    if (got_he) sv_setuv(HeVAL(got_he),uv);                             \
} STMT_END

#define HASH2INDEX(x,h2,xor_val,bucket_count) STMT_START {      \
        x= h2 ^ xor_val;                                                \
        /* see: https://stackoverflow.com/a/12996028                    \
         * but we could use any similar integer hash function. */       \
        x = ((x >> 16) ^ x) * 0x45d9f3b;                                \
        x = ((x >> 16) ^ x) * 0x45d9f3b;                                \
        x = ((x >> 16) ^ x);                                            \
        x %= bucket_count;                                              \
} STMT_END

#ifndef CHAR_BITS
#define CHAR_BITS 8
#endif

#define _BITSDECL(idx,bits) \
    const U64 bitpos= idx * bits;                           \
    const U64 bytepos= bitpos / CHAR_BITS;                  \
    const U8 shift= bitpos % CHAR_BITS;                     \
    const U8 bitmask= ( 1 << bits ) - 1

#define GETBITS(into,flags,idx,bits) STMT_START {           \
    _BITSDECL(idx,bits);                                    \
    into= ((flags)[bytepos] >> shift) & bitmask;            \
} STMT_END

#define SETBITS(value,flags,idx,bits) STMT_START {          \
    _BITSDECL(idx,bits);                                    \
    const U8 v= value;                                      \
    (flags)[bytepos] &= ~(bitmask << shift);                \
    (flags)[bytepos] |= ((v & bitmask) << shift);           \
} STMT_END

#define MAX_VARIANT 7
#define MIN_VARIANT 5

struct sv_with_hash {
    SV *sv;
    U32 hash;
};

typedef struct {
    struct sv_with_hash keyname_sv[COUNT_MPH_KEYSV];
} my_cxt_t;

struct mph_header {
    U32 magic_num;
    U32 variant;
    U32 num_buckets;
    U32 state_ofs;

    U32 table_ofs;
    U32 key_flags_ofs;
    U32 val_flags_ofs;
    U32 str_buf_ofs;

    union {
        U64 table_checksum;
        U64 general_flags;
        union {
            struct {
                U8 utf8_flags;
                U8 separator;
                U16 reserved_u16;
                U32 str_len_ofs;
            };
        };
    };
    union {
        U64 str_buf_checksum;
        union {
            U32 codepair_ofs;
            U32 reserved_u32_1;
        };
    };
};


#define BUCKET_FIELDS           \
    union {                     \
        U32 xor_val;            \
        I32 index;              \
    };                          \
    union {                     \
        U32 key_ofs;            \
        U32 k1_idx;             \
    };                          \
    union {                     \
        U32 val_ofs;            \
        U32 k2_idx;             \
    };                          \
    union {                     \
        struct {                \
            U16 key_len;        \
            U16 val_len;        \
        };                      \
        U32 k3_idx;             \
    }


struct mph_bucket {
    BUCKET_FIELDS;
};

struct mph_sorted_bucket {
    BUCKET_FIELDS;
    U32 sort_index;
};

struct mph_triple_bucket {
    BUCKET_FIELDS;
    U32 sort_index;
    U32 v_idx;
    I32 n1;
    I32 n2;
};

#define STR_LEN_PTR(mph)        ((struct str_len *)((U8 *)mph + mph->str_len_ofs))
#define STR_BUF_PTR(mph)        ((U8 *)mph + mph->str_buf_ofs)
#define KEY_FLAGS_PTR(mph)      ((U8 *)mph + mph->key_flags_ofs)
#define VAL_FLAGS_PTR(mph)      ((U8 *)mph + mph->val_flags_ofs)
#define STATE_PTR(mph)          ((U8 *)mph + mph->state_ofs)
#define BUCKET_PTR(mph)         ((struct mph_bucket *)((U8 *)mph + mph->table_ofs))
#define SORTED_BUCKET_PTR(mph)  ((struct mph_sorted_bucket *)((U8 *)mph + mph->table_ofs))
#define TRIPLE_BUCKET_PTR(mph)  ((struct mph_sorted_bucket *)((U8 *)mph + mph->table_ofs))


#define MPH_STR_LEN_HV_IDX 0
#define MPH_STR_LEN_NEXT_IDX 1
#define MPH_STR_LEN_UNDEF_IDX 2

#define MPH_NEG(n) ((-(n))-1)
#define MPH_FN_NEG(first_k_bucket,first_bucket,prev_bucket) MPH_NEG(prev_bucket - first_k_bucket)
#define MPH_FN_POS(first_k_bucket,first_bucket,prev_bucket) (first_k_bucket - prev_bucket)


struct str_len {
    U32 ofs;
    I32 len;
};

struct str_len_obj {
    HV *compressed_hv;
    HV *uncompressed_hv;
    U32 count;
    U32 next;
    U32 compressed_count;
    U32 uncompressed_count;
    U32 longest_compressed;
    U32 longest_uncompressed;
    U32 longest;
    struct compressor *compressor;
    struct str_len *str_len;
};



#define BUCKET_SIZE(variant) (                                \
     ((variant) == 5) ? sizeof(struct mph_bucket) :           \
     ((variant) == 6) ? sizeof(struct mph_sorted_bucket) :    \
                        sizeof(struct mph_triple_bucket)      \
)


#define mph_any_bucket mph_triple_bucket

#include "mph_hv_macro.h"
#include "mph_siphash.h"
#include "str_buf.h"
#include "trie2.h"

struct mph_obj {
    size_t bytes;
    struct mph_header *header;
    struct codepair_array codepair_array;
};

struct mph_multilevel {
    IV iter_idx;
    IV leftmost_idx;
    IV rightmost_idx;
    IV bucket_count;
    union {
        SV *prefix_sv;
        SV *p1_sv;
    };
    union {
        SV *prefix_utf8_sv;
        SV *p1_utf8_sv;
    };
    union {
        SV *prefix_latin1_sv;
        SV *p1_latin1_sv;
    };
    IV level;
    IV levels;
    IV fetch_key_first;
    char separator;
    char converted;
    SV *p2_sv;
    SV *p2_utf8_sv;
    SV *p2_latin1_sv;
    I32 k1_idx;
    I32 k2_idx;
};


UV _compute_xs(pTHX_ HV *self_hv, struct sv_with_hash *keyname_sv);
SV *_seed_state(pTHX_ SV *base_seed_sv);
UV _hash_with_state_sv(pTHX_ SV *str_sv, SV *state_sv);
IV _packed_xs(pTHX_ SV *buf_sv, U32 variant, SV *buf_length_sv, SV *state_sv, SV* comment_sv, U32 flags, AV *buckets_av, struct sv_with_hash *keyname_sv, AV * keys_av, SV *separator_sv);
SV *_mount_file(SV *file_sv, SV *error_sv, U32 flags);
void _mph_munmap(struct mph_obj *obj);
int lookup_bucket(pTHX_ struct mph_header *mph, U32 index, SV *key_sv, SV *val_sv);
int lookup_key(pTHX_ struct mph_header *mph, SV *key_sv, SV *val_sv);

IV find_prefix(pTHX_ struct mph_header *mph, SV *pfx_sv, IV l, IV r, I32 cmp_val);
IV find_first_last_prefix(pTHX_ struct mph_header *mph, SV *pfx_sv, IV l, IV r, IV *last);
IV find_last_prefix(pTHX_ struct mph_header *mph, SV *pfx_sv, IV l, IV r);
IV find_first_prefix(pTHX_ struct mph_header *mph, SV *pfx_sv, IV l, IV r);

I32 sv_prefix_cmp3(pTHX_ SV *l_sv, SV *r_sv, SV *r_sv_utf8);
I32 sv_cmp_hash(pTHX_ SV *a, SV *b);

IV triple_find_first_prefix(pTHX_ struct mph_obj *obj, SV *p1_sv, SV *p2_sv, SV *p3_sv, IV l, IV r);
IV triple_find_last_prefix(pTHX_ struct mph_obj *obj, SV *p1_sv, SV *p2_sv, SV *p3_sv, IV l, IV r);
IV triple_find_first_last_prefix(pTHX_ struct mph_obj *obj, SV *p1_sv, SV *p2_sv, SV *p3_sv, IV l, IV r, IV *last);

int triple_lookup_key_pvn(pTHX_ struct mph_obj *obj, struct mph_multilevel *ml, SV *full_key_sv, U8 *full_key_pv, STRLEN full_key_len, SV *val_sv, SV *leaf_sv);
void triple_set_val(struct mph_obj *obj, struct mph_triple_bucket *bucket, U32 bucket_idx, SV *val_sv);
void triple_set_key(struct mph_obj *obj, U32 str_len_idx, struct mph_triple_bucket *bucket, U32 bucket_idx, SV *key_sv);

void sv_set_from_str_len_idx( pTHX_ SV *got_sv, struct mph_header *mph, const U32 bucket_idx, const U32 str_len_idx );
void trigram_add_strs_from_av(pTHX_ AV *uncompressed_av, struct str_buf *str_buf);

MPH_STATIC_INLINE struct mph_triple_bucket *
last_bucket_by_k1(struct mph_triple_bucket *bucket) {
    I32 n= bucket->n1;
    return n > 0 ? bucket + n : bucket;
}

MPH_STATIC_INLINE struct mph_triple_bucket *
first_bucket_by_k1(struct mph_triple_bucket *bucket) {
    I32 n;
    struct mph_triple_bucket *last_bucket= last_bucket_by_k1(bucket);
    n= last_bucket->n1;
    return last_bucket + n + 1;
}

MPH_STATIC_INLINE struct mph_triple_bucket *
last_bucket_by_k2(struct mph_triple_bucket *bucket) {
    I32 n= bucket->n2;
    return n > 0 ? bucket + n : bucket;
}

MPH_STATIC_INLINE struct mph_triple_bucket *
first_bucket_by_k2(struct mph_triple_bucket *bucket) {
    I32 n;
    struct mph_triple_bucket *last_bucket= last_bucket_by_k2(bucket);
    n= last_bucket->n2;
    return last_bucket + n + 1;
}

MPH_STATIC_INLINE struct mph_triple_bucket *
prev_bucket_by_k1(struct mph_triple_bucket *bucket) {
    return first_bucket_by_k1(bucket) - 1;
}

MPH_STATIC_INLINE struct mph_triple_bucket *
next_bucket_by_k1(struct mph_triple_bucket *bucket) {
    return last_bucket_by_k1(bucket) + 1;
}

MPH_STATIC_INLINE struct mph_triple_bucket *
prev_bucket_by_k2(struct mph_triple_bucket *bucket) {
    return first_bucket_by_k2(bucket) - 1;
}

MPH_STATIC_INLINE struct mph_triple_bucket *
next_bucket_by_k2(struct mph_triple_bucket *bucket) {
    return last_bucket_by_k2(bucket) + 1;
}

