#ifndef _CTB_PBKDF2_H
#define _CTB_PBKDF2_H

#include <stddef.h>
#include <stdint.h>
#include "ctb_hmac_sha2.h"

#ifndef CTB_HMAC_SHA2_IMPLEMENTATION
	#define CTB_HMAC_SHA2_IMPLEMENTATION
#endif
#ifdef CTB_PBKDF2_NOPREFIX
	#define CTB_HMAC_SHA2_NOPREFIX
#endif
#include "ctb_sha2.h"


/* RFC 8018 PBKDF2 using HMAC-SHA2 family.
 * - password/salt may be NULL if corresponding length is zero.
 * - iterations >= 1, out_len >= 1.
 * - out must point to a writable buffer of out_len bytes.
 */

void ctb_pbkdf2_hmac_sha224(const uint8_t *password, size_t password_len,
                        const uint8_t *salt,     size_t salt_len,
                        uint32_t iterations,
                        uint8_t *out, size_t out_len);

void ctb_pbkdf2_hmac_sha256(const uint8_t *password, size_t password_len,
                        const uint8_t *salt,     size_t salt_len,
                        uint32_t iterations,
                        uint8_t *out, size_t out_len);

void ctb_pbkdf2_hmac_sha384(const uint8_t *password, size_t password_len,
                        const uint8_t *salt,     size_t salt_len,
                        uint32_t iterations,
                        uint8_t *out, size_t out_len);

void ctb_pbkdf2_hmac_sha512(const uint8_t *password, size_t password_len,
                        const uint8_t *salt,     size_t salt_len,
                        uint32_t iterations,
                        uint8_t *out, size_t out_len);

#ifdef CTB_PBKDF2_NOPREFIX
	#define pbkdf2_hmac_sha224 ctb_pbkdf2_hmac_sha224
	#define pbkdf2_hmac_sha256 ctb_pbkdf2_hmac_sha256
	#define pbkdf2_hmac_sha384 ctb_pbkdf2_hmac_sha394
	#define pbkdf2_hmac_sha512 ctb_pbkdf2_hmac_sha512
#endif


#ifdef CTB_PBKDF2_IMPLEMENTATION


/* write 32-bit big-endian integer */
static inline void be32(uint32_t x, uint8_t out[4]) {
    out[0] = (uint8_t)(x >> 24);
    out[1] = (uint8_t)(x >> 16);
    out[2] = (uint8_t)(x >> 8);
    out[3] = (uint8_t)(x);
}

/* Core PBKDF2 loop using the provided HMAC context API (init/reinit/update/final).
 * We use *_reinit so the keyed ipad/opad are reused for each HMAC invocation.
 */
#define DECL_PBKDF2_FN(NAME,                        \
                       HMAC_CTX_T,                  \
                       HMAC_INIT, HMAC_REINIT,      \
                       HMAC_UPDATE, HMAC_FINAL,     \
                       DIGEST_LEN)                  \
void NAME(const uint8_t *password, size_t password_len, \
          const uint8_t *salt,     size_t salt_len,     \
          uint32_t iterations,                          \
          uint8_t *out, size_t out_len)                 \
{                                                       \
    if (!out || out_len == 0 || iterations == 0)        \
        return;                                         \
                                                        \
    const size_t hLen = (size_t)(DIGEST_LEN);           \
    uint32_t blocks = (uint32_t)((out_len + hLen - 1) / hLen); \
                                                        \
    /* salt || INT_32_BE(i) buffer */                   \
    uint8_t *salt_cnt = (uint8_t*)malloc(salt_len + 4); \
    if (!salt_cnt) return;                              \
    if (salt_len) memcpy(salt_cnt, salt, salt_len);     \
                                                        \
    HMAC_CTX_T ctx;                                     \
    HMAC_INIT(&ctx, password, (unsigned int)password_len); \
                                                        \
    uint8_t *U = (uint8_t*)malloc(hLen);                \
    uint8_t *T = (uint8_t*)malloc(hLen);                \
    if (!U || !T) {                                     \
        free(salt_cnt);                                 \
        free(U); free(T);                               \
        return;                                         \
    }                                                   \
                                                        \
    size_t off = 0;                                     \
    for (uint32_t i = 1; i <= blocks; ++i) {            \
        be32(i, salt_cnt + salt_len);                   \
                                                        \
        /* U1 = PRF(P, S || INT(i)) */                  \
        HMAC_REINIT(&ctx);                              \
        HMAC_UPDATE(&ctx, salt_cnt, (unsigned int)(salt_len + 4)); \
        HMAC_FINAL(&ctx, U, (unsigned int)hLen);        \
        memcpy(T, U, hLen);                             \
                                                        \
        /* U2..Uc */                                    \
        for (uint32_t j = 2; j <= iterations; ++j) {    \
            HMAC_REINIT(&ctx);                          \
            HMAC_UPDATE(&ctx, U, (unsigned int)hLen);   \
            HMAC_FINAL(&ctx, U, (unsigned int)hLen);    \
            for (size_t k = 0; k < hLen; ++k)           \
                T[k] ^= U[k];                           \
        }                                               \
                                                        \
        size_t take = (out_len - off < hLen) ? (out_len - off) : hLen; \
        memcpy(out + off, T, take);                     \
        off += take;                                    \
    }                                                   \
                                                        \
    /* wipe & free */                                    \
    if (U) { memset(U, 0, hLen); free(U); }             \
    if (T) { memset(T, 0, hLen); free(T); }             \
    if (salt_cnt) { memset(salt_cnt, 0, salt_len + 4); free(salt_cnt); } \
}

/* Instantiate four PBKDF2 functions (HMAC-SHA2 variants) */
DECL_PBKDF2_FN(ctb_pbkdf2_hmac_sha224,
               hmac_sha224_ctx,
               hmac_sha224_init, hmac_sha224_reinit,
               hmac_sha224_update, hmac_sha224_final,
               SHA224_DIGEST_SIZE)

DECL_PBKDF2_FN(ctb_pbkdf2_hmac_sha256,
               hmac_sha256_ctx,
               hmac_sha256_init, hmac_sha256_reinit,
               hmac_sha256_update, hmac_sha256_final,
               SHA256_DIGEST_SIZE)

DECL_PBKDF2_FN(ctb_pbkdf2_hmac_sha384,
               hmac_sha384_ctx,
               hmac_sha384_init, hmac_sha384_reinit,
               hmac_sha384_update, hmac_sha384_final,
               SHA384_DIGEST_SIZE)

DECL_PBKDF2_FN(ctb_pbkdf2_hmac_sha512,
               hmac_sha512_ctx,
               hmac_sha512_init, hmac_sha512_reinit,
               hmac_sha512_update, hmac_sha512_final,
               SHA512_DIGEST_SIZE)

#endif


#endif
