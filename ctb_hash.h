#ifndef _CTB_HASH_H
#define _CTB_HASH_H

#include <stddef.h>
#include <stdint.h>

/* =========================================================================
   1. SHA1 API
   ========================================================================= */

typedef struct
{
	uint32_t state[5];
	uint32_t count[2];
	unsigned char buffer[64];
} ctb_sha1_ctx;

void ctb_sha1_init(ctb_sha1_ctx * context);
void ctb_sha1_update(ctb_sha1_ctx * context, const unsigned char *data, uint32_t len);
void ctb_sha1_final(unsigned char digest[20], ctb_sha1_ctx * context);
void ctb_sha1(char *hash_out, const char *str, uint32_t len);

/* =========================================================================
   2. RIPEMD160 API
   ========================================================================= */

#define _CTB_RIPEMD160_BLOCK_LENGTH 64
#define _CTB_RIPEMD160_DIGEST_LENGTH 20

typedef struct _ctb_ripemd160_ctx
{
	uint32_t total[2];                      /*!< number of bytes processed  */
	uint32_t state[5];                      /*!< intermediate digest state  */
	uint8_t buffer[_CTB_RIPEMD160_BLOCK_LENGTH]; /*!< data block being processed */
} ctb_ripemd160_ctx;

void ctb_ripemd160_init(ctb_ripemd160_ctx *ctx);
void ctb_ripemd160_update(ctb_ripemd160_ctx *ctx, const uint8_t *input, uint32_t ilen);
void ctb_ripemd160_final(ctb_ripemd160_ctx *ctx, uint8_t output[_CTB_RIPEMD160_DIGEST_LENGTH]);
void ctb_ripemd160(const uint8_t *msg, uint32_t msg_len, uint8_t hash[_CTB_RIPEMD160_DIGEST_LENGTH]);


/* =========================================================================
   3. SHA2 API (SHA224, SHA256, SHA384, SHA512)
   ========================================================================= */

#define _CTB_SHA224_DIGEST_SIZE ( 224 / 8)
#define _CTB_SHA256_DIGEST_SIZE ( 256 / 8)
#define _CTB_SHA384_DIGEST_SIZE ( 384 / 8)
#define _CTB_SHA512_DIGEST_SIZE ( 512 / 8)

#define _CTB_SHA256_BLOCK_SIZE  ( 512 / 8)
#define _CTB_SHA512_BLOCK_SIZE  (1024 / 8)
#define _CTB_SHA384_BLOCK_SIZE  _CTB_SHA512_BLOCK_SIZE
#define _CTB_SHA224_BLOCK_SIZE  _CTB_SHA256_BLOCK_SIZE

#ifndef _CTB_SHA2_TYPES
#define _CTB_SHA2_TYPES
typedef unsigned char uint8;
typedef unsigned int  uint32;
typedef unsigned long long uint64;
#endif

typedef struct
{
	unsigned int tot_len;
	unsigned int len;
	unsigned char block[2 * _CTB_SHA256_BLOCK_SIZE];
	uint32 h[8];
} ctb_sha256_ctx;

typedef struct
{
	unsigned int tot_len;
	unsigned int len;
	unsigned char block[2 * _CTB_SHA512_BLOCK_SIZE];
	uint64 h[8];
} ctb_sha512_ctx;

typedef ctb_sha512_ctx ctb_sha384_ctx;
typedef ctb_sha256_ctx ctb_sha224_ctx;

void ctb_sha224_init(ctb_sha224_ctx *ctx);
void ctb_sha224_update(ctb_sha224_ctx *ctx, const unsigned char *message, unsigned int len);
void ctb_sha224_final(ctb_sha224_ctx *ctx, unsigned char *digest);
void ctb_sha224(const unsigned char *message, unsigned int len, unsigned char *digest);

void ctb_sha256_init(ctb_sha256_ctx * ctx);
void ctb_sha256_update(ctb_sha256_ctx *ctx, const unsigned char *message, unsigned int len);
void ctb_sha256_final(ctb_sha256_ctx *ctx, unsigned char *digest);
void ctb_sha256(const unsigned char *message, unsigned int len, unsigned char *digest);

void ctb_sha384_init(ctb_sha384_ctx *ctx);
void ctb_sha384_update(ctb_sha384_ctx *ctx, const unsigned char *message, unsigned int len);
void ctb_sha384_final(ctb_sha384_ctx *ctx, unsigned char *digest);
void ctb_sha384(const unsigned char *message, unsigned int len, unsigned char *digest);

void ctb_sha512_init(ctb_sha512_ctx *ctx);
void ctb_sha512_update(ctb_sha512_ctx *ctx, const unsigned char *message, unsigned int len);
void ctb_sha512_final(ctb_sha512_ctx *ctx, unsigned char *digest);
void ctb_sha512(const unsigned char *message, unsigned int len, unsigned char *digest);


/* =========================================================================
   4. HMAC API
   ========================================================================= */

typedef struct
{
	ctb_sha224_ctx ctx_inside;
	ctb_sha224_ctx ctx_outside;

	/* for hmac_reinit */
	ctb_sha224_ctx ctx_inside_reinit;
	ctb_sha224_ctx ctx_outside_reinit;

	unsigned char block_ipad[_CTB_SHA224_BLOCK_SIZE];
	unsigned char block_opad[_CTB_SHA224_BLOCK_SIZE];
} ctb_hmac_sha224_ctx;

typedef struct
{
	ctb_sha256_ctx ctx_inside;
	ctb_sha256_ctx ctx_outside;

	/* for hmac_reinit */
	ctb_sha256_ctx ctx_inside_reinit;
	ctb_sha256_ctx ctx_outside_reinit;

	unsigned char block_ipad[_CTB_SHA256_BLOCK_SIZE];
	unsigned char block_opad[_CTB_SHA256_BLOCK_SIZE];
} ctb_hmac_sha256_ctx;

typedef struct
{
	ctb_sha384_ctx ctx_inside;
	ctb_sha384_ctx ctx_outside;

	/* for hmac_reinit */
	ctb_sha384_ctx ctx_inside_reinit;
	ctb_sha384_ctx ctx_outside_reinit;

	unsigned char block_ipad[_CTB_SHA384_BLOCK_SIZE];
	unsigned char block_opad[_CTB_SHA384_BLOCK_SIZE];
} ctb_hmac_sha384_ctx;

typedef struct
{
	ctb_sha512_ctx ctx_inside;
	ctb_sha512_ctx ctx_outside;

	/* for hmac_reinit */
	ctb_sha512_ctx ctx_inside_reinit;
	ctb_sha512_ctx ctx_outside_reinit;

	unsigned char block_ipad[_CTB_SHA512_BLOCK_SIZE];
	unsigned char block_opad[_CTB_SHA512_BLOCK_SIZE];
} ctb_hmac_sha512_ctx;

void ctb_hmac_sha224_init(ctb_hmac_sha224_ctx *ctx, const unsigned char *key, unsigned int key_size);
void ctb_hmac_sha224_reinit(ctb_hmac_sha224_ctx *ctx);
void ctb_hmac_sha224_update(ctb_hmac_sha224_ctx *ctx, const unsigned char *message, unsigned int message_len);
void ctb_hmac_sha224_final(ctb_hmac_sha224_ctx *ctx, unsigned char *mac, unsigned int mac_size);
void ctb_hmac_sha224(const unsigned char *key, unsigned int key_size,
					 const unsigned char *message, unsigned int message_len,
					 unsigned char *mac, unsigned mac_size);

void ctb_hmac_sha256_init(ctb_hmac_sha256_ctx *ctx, const unsigned char *key, unsigned int key_size);
void ctb_hmac_sha256_reinit(ctb_hmac_sha256_ctx *ctx);
void ctb_hmac_sha256_update(ctb_hmac_sha256_ctx *ctx, const unsigned char *message, unsigned int message_len);
void ctb_hmac_sha256_final(ctb_hmac_sha256_ctx *ctx, unsigned char *mac, unsigned int mac_size);
void ctb_hmac_sha256(const unsigned char *key, unsigned int key_size,
					 const unsigned char *message, unsigned int message_len,
					 unsigned char *mac, unsigned mac_size);

void ctb_hmac_sha384_init(ctb_hmac_sha384_ctx *ctx, const unsigned char *key, unsigned int key_size);
void ctb_hmac_sha384_reinit(ctb_hmac_sha384_ctx *ctx);
void ctb_hmac_sha384_update(ctb_hmac_sha384_ctx *ctx, const unsigned char *message, unsigned int message_len);
void ctb_hmac_sha384_final(ctb_hmac_sha384_ctx *ctx, unsigned char *mac, unsigned int mac_size);
void ctb_hmac_sha384(const unsigned char *key, unsigned int key_size,
					 const unsigned char *message, unsigned int message_len,
					 unsigned char *mac, unsigned mac_size);

void ctb_hmac_sha512_init(ctb_hmac_sha512_ctx *ctx, const unsigned char *key, unsigned int key_size);
void ctb_hmac_sha512_reinit(ctb_hmac_sha512_ctx *ctx);
void ctb_hmac_sha512_update(ctb_hmac_sha512_ctx *ctx, const unsigned char *message, unsigned int message_len);
void ctb_hmac_sha512_final(ctb_hmac_sha512_ctx *ctx, unsigned char *mac, unsigned int mac_size);
void ctb_hmac_sha512(const unsigned char *key, unsigned int key_size,
					 const unsigned char *message, unsigned int message_len,
					 unsigned char *mac, unsigned mac_size);


/* =========================================================================
   5. PBKDF2 API
   ========================================================================= */

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

#ifdef CTB_HASH_NOPREFIX
/* SHA1 */
typedef	ctb_sha1_ctx	sha1_ctx;
#define	sha1_transform	ctb_sha1_transform
#define	sha1_init		ctb_sha1_init
#define	sha1_update		ctb_sha1_update
#define	sha1_final		ctb_sha1_final
#define	sha1			ctb_sha1

/* RIPEMD160 */
typedef ctb_ripemd160_ctx	ripemd160_ctx;
#define ripemd160_init		ctb_ripemd160_init
#define ripemd160_update	ctb_ripemd160_update
#define ripemd160_final		ctb_ripemd160_final
#define ripemd160			ctb_ripemd160

/* SHA2 */
typedef ctb_sha224_ctx	sha224_ctx;
#define sha224_init		ctb_sha224_init
#define sha224_update	ctb_sha224_update
#define sha224_final	ctb_sha224_final
#define sha224			ctb_sha224

typedef ctb_sha256_ctx	sha256_ctx;
#define sha256_init		ctb_sha256_init
#define sha256_update	ctb_sha256_update
#define sha256_final	ctb_sha256_final
#define sha256			ctb_sha256

typedef ctb_sha384_ctx	sha384_ctx;
#define sha384_init		ctb_sha384_init
#define sha384_update	ctb_sha384_update
#define sha384_final	ctb_sha384_final
#define sha384			ctb_sha384

typedef ctb_sha512_ctx	sha512_ctx;
#define sha512_init		ctb_sha512_init
#define sha512_update	ctb_sha512_update
#define sha512_final	ctb_sha512_final
#define sha512			ctb_sha512

/* HMAC */
typedef ctb_hmac_sha224_ctx	hmac_sha224_ctx;
#define hmac_sha224_init	ctb_hmac_sha224_init
#define hmac_sha224_reinit	ctb_hmac_sha224_reinit
#define hmac_sha224_update	ctb_hmac_sha224_update
#define hmac_sha224_final	ctb_hmac_sha224_final
#define hmac_sha224			ctb_hmac_sha224

typedef ctb_hmac_sha256_ctx	hmac_sha256_ctx;
#define hmac_sha256_init	ctb_hmac_sha256_init
#define hmac_sha256_reinit	ctb_hmac_sha256_reinit
#define hmac_sha256_update	ctb_hmac_sha256_update
#define hmac_sha256_final	ctb_hmac_sha256_final
#define hmac_sha256			ctb_hmac_sha256

typedef ctb_hmac_sha384_ctx	hmac_sha384_ctx;
#define hmac_sha384_init	ctb_hmac_sha384_init
#define hmac_sha384_reinit	ctb_hmac_sha384_reinit
#define hmac_sha384_update	ctb_hmac_sha384_update
#define hmac_sha384_final	ctb_hmac_sha384_final
#define hmac_sha384			ctb_hmac_sha384

typedef ctb_hmac_sha512_ctx	hmac_sha512_ctx;
#define hmac_sha512_init	ctb_hmac_sha512_init
#define hmac_sha512_reinit	ctb_hmac_sha512_reinit
#define hmac_sha512_update	ctb_hmac_sha512_update
#define hmac_sha512_final	ctb_hmac_sha512_final
#define hmac_sha512			ctb_hmac_sha512

/* PBKDF2 */
#define pbkdf2_hmac_sha224 ctb_pbkdf2_hmac_sha224
#define pbkdf2_hmac_sha256 ctb_pbkdf2_hmac_sha256
#define pbkdf2_hmac_sha384 ctb_pbkdf2_hmac_sha384
#define pbkdf2_hmac_sha512 ctb_pbkdf2_hmac_sha512

#endif

#endif // _CTB_CRYPTO_H


#ifdef CTB_HASH_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* =========================================================================
   SHA1 IMPLEMENTATION
   ========================================================================= */
#define CTB_SHA1_HANDSOFF

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))

/* blk0() and blk() perform the initial expand. */
/* I got the idea of expanding during the round function from SSLeay */
#define blk0_le(i) (block->l[i] = (rol(block->l[i],24)&0xFF00FF00) \
	|(rol(block->l[i],8)&0x00FF00FF))
#define blk0_be(i) block->l[i]
#if BYTE_ORDER == LITTLE_ENDIAN
#define blk0(i) blk0_le(i)
#elif BYTE_ORDER == BIG_ENDIAN
#define blk0(i) blk0_be(i)
#else
/* Fall back to a runtime endian check */
static const union {
	long l;
	char c;
} sha1_endian = { 1 };
#define blk0(i) (sha1_endian.c == 0 ? blk0_be(i) : blk0_le(i))
#endif

#define blk(i) (block->l[i&15] = rol(block->l[(i+13)&15]^block->l[(i+8)&15] \
									 ^block->l[(i+2)&15]^block->l[i&15],1))

/* (R0+R1), R2, R3, R4 are the different operations used in ctb_sha1 */
#define R0(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=((w&(x^y))^y)+blk(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=(((w|x)&y)|(w&x))+blk(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=(w^x^y)+blk(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);


/* Hash a single 512-bit block. This is the core of the algorithm. */

void ctb_sha1_transform(
	uint32_t state[5],
	const unsigned char buffer[64]
)
{
	uint32_t a, b, c, d, e;

	typedef union
	{
		unsigned char c[64];
		uint32_t l[16];
	} CHAR64LONG16;

#ifdef CTB_SHA1_HANDSOFF
	CHAR64LONG16 block[1];      /* use array to appear as a pointer */

	memcpy(block, buffer, 64);
#else
	/* The following had better never be used because it causes the
	 * pointer-to-const buffer to be cast into a pointer to non-const.
	 * And the result is written through.  I threw a "const" in, hoping
	 * this will cause a diagnostic.
	 */
	CHAR64LONG16 *block = (const CHAR64LONG16 *) buffer;
#endif
	/* Copy context->state[] to working vars */
	a = state[0];
	b = state[1];
	c = state[2];
	d = state[3];
	e = state[4];
	/* 4 rounds of 20 operations each. Loop unrolled. */
	R0(a, b, c, d, e, 0);
	R0(e, a, b, c, d, 1);
	R0(d, e, a, b, c, 2);
	R0(c, d, e, a, b, 3);
	R0(b, c, d, e, a, 4);
	R0(a, b, c, d, e, 5);
	R0(e, a, b, c, d, 6);
	R0(d, e, a, b, c, 7);
	R0(c, d, e, a, b, 8);
	R0(b, c, d, e, a, 9);
	R0(a, b, c, d, e, 10);
	R0(e, a, b, c, d, 11);
	R0(d, e, a, b, c, 12);
	R0(c, d, e, a, b, 13);
	R0(b, c, d, e, a, 14);
	R0(a, b, c, d, e, 15);
	R1(e, a, b, c, d, 16);
	R1(d, e, a, b, c, 17);
	R1(c, d, e, a, b, 18);
	R1(b, c, d, e, a, 19);
	R2(a, b, c, d, e, 20);
	R2(e, a, b, c, d, 21);
	R2(d, e, a, b, c, 22);
	R2(c, d, e, a, b, 23);
	R2(b, c, d, e, a, 24);
	R2(a, b, c, d, e, 25);
	R2(e, a, b, c, d, 26);
	R2(d, e, a, b, c, 27);
	R2(c, d, e, a, b, 28);
	R2(b, c, d, e, a, 29);
	R2(a, b, c, d, e, 30);
	R2(e, a, b, c, d, 31);
	R2(d, e, a, b, c, 32);
	R2(c, d, e, a, b, 33);
	R2(b, c, d, e, a, 34);
	R2(a, b, c, d, e, 35);
	R2(e, a, b, c, d, 36);
	R2(d, e, a, b, c, 37);
	R2(c, d, e, a, b, 38);
	R2(b, c, d, e, a, 39);
	R3(a, b, c, d, e, 40);
	R3(e, a, b, c, d, 41);
	R3(d, e, a, b, c, 42);
	R3(c, d, e, a, b, 43);
	R3(b, c, d, e, a, 44);
	R3(a, b, c, d, e, 45);
	R3(e, a, b, c, d, 46);
	R3(d, e, a, b, c, 47);
	R3(c, d, e, a, b, 48);
	R3(b, c, d, e, a, 49);
	R3(a, b, c, d, e, 50);
	R3(e, a, b, c, d, 51);
	R3(d, e, a, b, c, 52);
	R3(c, d, e, a, b, 53);
	R3(b, c, d, e, a, 54);
	R3(a, b, c, d, e, 55);
	R3(e, a, b, c, d, 56);
	R3(d, e, a, b, c, 57);
	R3(c, d, e, a, b, 58);
	R3(b, c, d, e, a, 59);
	R4(a, b, c, d, e, 60);
	R4(e, a, b, c, d, 61);
	R4(d, e, a, b, c, 62);
	R4(c, d, e, a, b, 63);
	R4(b, c, d, e, a, 64);
	R4(a, b, c, d, e, 65);
	R4(e, a, b, c, d, 66);
	R4(d, e, a, b, c, 67);
	R4(c, d, e, a, b, 68);
	R4(b, c, d, e, a, 69);
	R4(a, b, c, d, e, 70);
	R4(e, a, b, c, d, 71);
	R4(d, e, a, b, c, 72);
	R4(c, d, e, a, b, 73);
	R4(b, c, d, e, a, 74);
	R4(a, b, c, d, e, 75);
	R4(e, a, b, c, d, 76);
	R4(d, e, a, b, c, 77);
	R4(c, d, e, a, b, 78);
	R4(b, c, d, e, a, 79);
	/* Add the working vars back into context.state[] */
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	state[4] += e;
	/* Wipe variables */
	a = b = c = d = e = 0;
#ifdef CTB_SHA1_HANDSOFF
	memset(block, '\0', sizeof(block));
#endif
}


/* ctb_sha1_init - Initialize new context */

void ctb_sha1_init(
	ctb_sha1_ctx * context
)
{
	/* ctb_sha1 initialization constants */
	context->state[0] = 0x67452301;
	context->state[1] = 0xEFCDAB89;
	context->state[2] = 0x98BADCFE;
	context->state[3] = 0x10325476;
	context->state[4] = 0xC3D2E1F0;
	context->count[0] = context->count[1] = 0;
}


/* Run your data through this. */

void ctb_sha1_update(
	ctb_sha1_ctx * context,
	const unsigned char *data,
	uint32_t len
)
{
	uint32_t i;

	uint32_t j;

	j = context->count[0];
	if ((context->count[0] += len << 3) < j)
		context->count[1]++;
	context->count[1] += (len >> 29);
	j = (j >> 3) & 63;
	if ((j + len) > 63)
	{
		memcpy(&context->buffer[j], data, (i = 64 - j));
		ctb_sha1_transform(context->state, context->buffer);
		for (; i + 63 < len; i += 64)
		{
			ctb_sha1_transform(context->state, &data[i]);
		}
		j = 0;
	}
	else
	i = 0;
	memcpy(&context->buffer[j], &data[i], len - i);
}


/* Add padding and return the message digest. */

void ctb_sha1_final(
	unsigned char digest[20],
	ctb_sha1_ctx * context
)
{
	unsigned i;

	unsigned char finalcount[8];

	unsigned char c;

#if 0    /* untested "improvement" by DHR */
	/* Convert context->count to a sequence of bytes
	 * in finalcount.  Second element first, but
	 * big-endian order within element.
	 * But we do it all backwards.
	 */
	unsigned char *fcp = &finalcount[8];

	for (i = 0; i < 2; i++)
	{
		uint32_t t = context->count[i];

		int j;

		for (j = 0; j < 4; t >>= 8, j++)
			*--fcp = (unsigned char) t}
#else
	for (i = 0; i < 8; i++)
	{
		finalcount[i] = (unsigned char) ((context->count[(i >= 4 ? 0 : 1)] >> ((3 - (i & 3)) * 8)) & 255);      /* Endian independent */
	}
#endif
	c = 0200;
	ctb_sha1_update(context, &c, 1);
	while ((context->count[0] & 504) != 448)
	{
		c = 0000;
		ctb_sha1_update(context, &c, 1);
	}
	ctb_sha1_update(context, finalcount, 8); /* Should cause a ctb_sha1_transform() */
	for (i = 0; i < 20; i++)
	{
		digest[i] = (unsigned char)
			((context->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
	}
	/* Wipe variables */
	memset(context, '\0', sizeof(*context));
	memset(&finalcount, '\0', sizeof(finalcount));
}

void ctb_sha1(
	char *hash_out,
	const char *str,
	uint32_t len)
{
	ctb_sha1_ctx ctx;
	unsigned int ii;

	ctb_sha1_init(&ctx);
	for (ii=0; ii<len; ii+=1)
		ctb_sha1_update(&ctx, (const unsigned char*)str + ii, 1);
	ctb_sha1_final((unsigned char *)hash_out, &ctx);
}

#undef R0
#undef R1
#undef R2
#undef R3
#undef R4
#undef rol
#undef blk0
#undef blk

/* =========================================================================
   RIPEMD160 IMPLEMENTATION
   ========================================================================= */

#ifndef __STDC_WANT_LIB_EXT1__
#define __STDC_WANT_LIB_EXT1__ 1  // C11's bounds-checking interface.
#endif

#ifndef _DEFAULT_SOURCE
#  define _DEFAULT_SOURCE 1
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __unix__
#include <strings.h>
#include <sys/param.h>
#endif

// C11's bounds-checking interface.
#if defined(__STDC_LIB_EXT1__)
#define HAVE_MEMSET_S 1
#endif

// GNU C Library version 2.25 or later.
#if defined(__GLIBC__) && \
(__GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ >= 25))
#define HAVE_EXPLICIT_BZERO 1
#endif

// Newlib
#if defined(__NEWLIB__)
#define HAVE_EXPLICIT_BZERO 1
#endif

// FreeBSD version 11.0 or later.
#if defined(__FreeBSD__) && __FreeBSD_version >= 1100037
#define HAVE_EXPLICIT_BZERO 1
#endif

// OpenBSD version 5.5 or later.
#if defined(__OpenBSD__) && OpenBSD >= 201405
#define HAVE_EXPLICIT_BZERO 1
#endif

// NetBSD version 7.2 or later.
#if defined(__NetBSD__) && __NetBSD_Version__ >= 702000000
#define HAVE_EXPLICIT_MEMSET 1
#endif

// Helper to zero memory (internal usage only)
static void ctb__memzero(void *const pnt, const size_t len) {
#ifdef _WIN32
	SecureZeroMemory(pnt, len);
#elif defined(HAVE_MEMSET_S)
	memset_s(pnt, (rsize_t)len, 0, (rsize_t)len);
#elif defined(HAVE_EXPLICIT_BZERO)
	explicit_bzero(pnt, len);
#elif defined(HAVE_EXPLICIT_MEMSET)
	explicit_memset(pnt, 0, len);
#else
	volatile unsigned char *volatile pnt_ = (volatile unsigned char *volatile)pnt;
	size_t i = (size_t)0U;

	while (i < len) {
		pnt_[i++] = 0U;
	}
#endif
}

/*
 * 32-bit integer manipulation macros (little endian)
 */
#ifndef GET_UINT32_LE
#define GET_UINT32_LE(n,b,i)                            \
{                                                       \
	(n) = ( (uint32_t) (b)[(i)    ]       )             \
		| ( (uint32_t) (b)[(i) + 1] <<  8 )             \
		| ( (uint32_t) (b)[(i) + 2] << 16 )             \
		| ( (uint32_t) (b)[(i) + 3] << 24 );            \
}
#endif

#ifndef PUT_UINT32_LE
#define PUT_UINT32_LE(n,b,i)                                    \
{                                                               \
	(b)[(i)    ] = (uint8_t) ( ( (n)       ) & 0xFF );    \
	(b)[(i) + 1] = (uint8_t) ( ( (n) >>  8 ) & 0xFF );    \
	(b)[(i) + 2] = (uint8_t) ( ( (n) >> 16 ) & 0xFF );    \
	(b)[(i) + 3] = (uint8_t) ( ( (n) >> 24 ) & 0xFF );    \
}
#endif

/*
 * RIPEMD-160 context setup
 */
void ctb_ripemd160_init(ctb_ripemd160_ctx *ctx)
{
	ctb__memzero(ctx, sizeof(ctb_ripemd160_ctx));
	ctx->total[0] = 0;
	ctx->total[1] = 0;
	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
	ctx->state[4] = 0xC3D2E1F0;
}

#if !defined(MBEDTLS_RIPEMD160_PROCESS_ALT)
/*
 * Process one block
 */
void ripemd160_process( ctb_ripemd160_ctx *ctx, const uint8_t data[_CTB_RIPEMD160_BLOCK_LENGTH] )
{
	uint32_t A, B, C, D, E, Ap, Bp, Cp, Dp, Ep, X[16];

	GET_UINT32_LE( X[ 0], data,  0 );
	GET_UINT32_LE( X[ 1], data,  4 );
	GET_UINT32_LE( X[ 2], data,  8 );
	GET_UINT32_LE( X[ 3], data, 12 );
	GET_UINT32_LE( X[ 4], data, 16 );
	GET_UINT32_LE( X[ 5], data, 20 );
	GET_UINT32_LE( X[ 6], data, 24 );
	GET_UINT32_LE( X[ 7], data, 28 );
	GET_UINT32_LE( X[ 8], data, 32 );
	GET_UINT32_LE( X[ 9], data, 36 );
	GET_UINT32_LE( X[10], data, 40 );
	GET_UINT32_LE( X[11], data, 44 );
	GET_UINT32_LE( X[12], data, 48 );
	GET_UINT32_LE( X[13], data, 52 );
	GET_UINT32_LE( X[14], data, 56 );
	GET_UINT32_LE( X[15], data, 60 );

	A = Ap = ctx->state[0];
	B = Bp = ctx->state[1];
	C = Cp = ctx->state[2];
	D = Dp = ctx->state[3];
	E = Ep = ctx->state[4];

#define F1( x, y, z )   ( x ^ y ^ z )
#define F2( x, y, z )   ( ( x & y ) | ( ~x & z ) )
#define F3( x, y, z )   ( ( x | ~y ) ^ z )
#define F4( x, y, z )   ( ( x & z ) | ( y & ~z ) )
#define F5( x, y, z )   ( x ^ ( y | ~z ) )

#define S( x, n ) ( ( x << n ) | ( x >> (32 - n) ) )

#define P( a, b, c, d, e, r, s, f, k )      \
	a += f( b, c, d ) + X[r] + k;           \
	a = S( a, s ) + e;                      \
	c = S( c, 10 );

#define P2( a, b, c, d, e, r, s, rp, sp )   \
	P( a, b, c, d, e, r, s, F, K );         \
	P( a ## p, b ## p, c ## p, d ## p, e ## p, rp, sp, Fp, Kp );

#define F   F1
#define K   0x00000000
#define Fp  F5
#define Kp  0x50A28BE6
	P2( A, B, C, D, E,  0, 11,  5,  8 );
	P2( E, A, B, C, D,  1, 14, 14,  9 );
	P2( D, E, A, B, C,  2, 15,  7,  9 );
	P2( C, D, E, A, B,  3, 12,  0, 11 );
	P2( B, C, D, E, A,  4,  5,  9, 13 );
	P2( A, B, C, D, E,  5,  8,  2, 15 );
	P2( E, A, B, C, D,  6,  7, 11, 15 );
	P2( D, E, A, B, C,  7,  9,  4,  5 );
	P2( C, D, E, A, B,  8, 11, 13,  7 );
	P2( B, C, D, E, A,  9, 13,  6,  7 );
	P2( A, B, C, D, E, 10, 14, 15,  8 );
	P2( E, A, B, C, D, 11, 15,  8, 11 );
	P2( D, E, A, B, C, 12,  6,  1, 14 );
	P2( C, D, E, A, B, 13,  7, 10, 14 );
	P2( B, C, D, E, A, 14,  9,  3, 12 );
	P2( A, B, C, D, E, 15,  8, 12,  6 );
	#undef F
	#undef K
	#undef Fp
	#undef Kp

#define F   F2
#define K   0x5A827999
#define Fp  F4
#define Kp  0x5C4DD124
	P2( E, A, B, C, D,  7,  7,  6,  9 );
	P2( D, E, A, B, C,  4,  6, 11, 13 );
	P2( C, D, E, A, B, 13,  8,  3, 15 );
	P2( B, C, D, E, A,  1, 13,  7,  7 );
	P2( A, B, C, D, E, 10, 11,  0, 12 );
	P2( E, A, B, C, D,  6,  9, 13,  8 );
	P2( D, E, A, B, C, 15,  7,  5,  9 );
	P2( C, D, E, A, B,  3, 15, 10, 11 );
	P2( B, C, D, E, A, 12,  7, 14,  7 );
	P2( A, B, C, D, E,  0, 12, 15,  7 );
	P2( E, A, B, C, D,  9, 15,  8, 12 );
	P2( D, E, A, B, C,  5,  9, 12,  7 );
	P2( C, D, E, A, B,  2, 11,  4,  6 );
	P2( B, C, D, E, A, 14,  7,  9, 15 );
	P2( A, B, C, D, E, 11, 13,  1, 13 );
	P2( E, A, B, C, D,  8, 12,  2, 11 );
	#undef F
	#undef K
	#undef Fp
	#undef Kp

#define F   F3
#define K   0x6ED9EBA1
#define Fp  F3
#define Kp  0x6D703EF3
	P2( D, E, A, B, C,  3, 11, 15,  9 );
	P2( C, D, E, A, B, 10, 13,  5,  7 );
	P2( B, C, D, E, A, 14,  6,  1, 15 );
	P2( A, B, C, D, E,  4,  7,  3, 11 );
	P2( E, A, B, C, D,  9, 14,  7,  8 );
	P2( D, E, A, B, C, 15,  9, 14,  6 );
	P2( C, D, E, A, B,  8, 13,  6,  6 );
	P2( B, C, D, E, A,  1, 15,  9, 14 );
	P2( A, B, C, D, E,  2, 14, 11, 12 );
	P2( E, A, B, C, D,  7,  8,  8, 13 );
	P2( D, E, A, B, C,  0, 13, 12,  5 );
	P2( C, D, E, A, B,  6,  6,  2, 14 );
	P2( B, C, D, E, A, 13,  5, 10, 13 );
	P2( A, B, C, D, E, 11, 12,  0, 13 );
	P2( E, A, B, C, D,  5,  7,  4,  7 );
	P2( D, E, A, B, C, 12,  5, 13,  5 );
	#undef F
	#undef K
	#undef Fp
	#undef Kp

#define F   F4
#define K   0x8F1BBCDC
#define Fp  F2
#define Kp  0x7A6D76E9
	P2( C, D, E, A, B,  1, 11,  8, 15 );
	P2( B, C, D, E, A,  9, 12,  6,  5 );
	P2( A, B, C, D, E, 11, 14,  4,  8 );
	P2( E, A, B, C, D, 10, 15,  1, 11 );
	P2( D, E, A, B, C,  0, 14,  3, 14 );
	P2( C, D, E, A, B,  8, 15, 11, 14 );
	P2( B, C, D, E, A, 12,  9, 15,  6 );
	P2( A, B, C, D, E,  4,  8,  0, 14 );
	P2( E, A, B, C, D, 13,  9,  5,  6 );
	P2( D, E, A, B, C,  3, 14, 12,  9 );
	P2( C, D, E, A, B,  7,  5,  2, 12 );
	P2( B, C, D, E, A, 15,  6, 13,  9 );
	P2( A, B, C, D, E, 14,  8,  9, 12 );
	P2( E, A, B, C, D,  5,  6,  7,  5 );
	P2( D, E, A, B, C,  6,  5, 10, 15 );
	P2( C, D, E, A, B,  2, 12, 14,  8 );
	#undef F
	#undef K
	#undef Fp
	#undef Kp

#define F   F5
#define K   0xA953FD4E
#define Fp  F1
#define Kp  0x00000000
	P2( B, C, D, E, A,  4,  9, 12,  8 );
	P2( A, B, C, D, E,  0, 15, 15,  5 );
	P2( E, A, B, C, D,  5,  5, 10, 12 );
	P2( D, E, A, B, C,  9, 11,  4,  9 );
	P2( C, D, E, A, B,  7,  6,  1, 12 );
	P2( B, C, D, E, A, 12,  8,  5,  5 );
	P2( A, B, C, D, E,  2, 13,  8, 14 );
	P2( E, A, B, C, D, 10, 12,  7,  6 );
	P2( D, E, A, B, C, 14,  5,  6,  8 );
	P2( C, D, E, A, B,  1, 12,  2, 13 );
	P2( B, C, D, E, A,  3, 13, 13,  6 );
	P2( A, B, C, D, E,  8, 14, 14,  5 );
	P2( E, A, B, C, D, 11, 11,  0, 15 );
	P2( D, E, A, B, C,  6,  8,  3, 13 );
	P2( C, D, E, A, B, 15,  5,  9, 11 );
	P2( B, C, D, E, A, 13,  6, 11, 11 );
	#undef F
	#undef K
	#undef Fp
	#undef Kp

	C             = ctx->state[1] + C + Dp;
	ctx->state[1] = ctx->state[2] + D + Ep;
	ctx->state[2] = ctx->state[3] + E + Ap;
	ctx->state[3] = ctx->state[4] + A + Bp;
	ctx->state[4] = ctx->state[0] + B + Cp;
	ctx->state[0] = C;
}
#endif /* !MBEDTLS_RIPEMD160_PROCESS_ALT */

/*
 * RIPEMD-160 process buffer
 */
void ctb_ripemd160_update( ctb_ripemd160_ctx *ctx, const uint8_t *input, uint32_t ilen )
{
	uint32_t fill;
	uint32_t left;

	if( ilen == 0 )
		return;

	left = ctx->total[0] & 0x3F;
	fill = _CTB_RIPEMD160_BLOCK_LENGTH - left;

	ctx->total[0] += (uint32_t) ilen;
	ctx->total[0] &= 0xFFFFFFFF;

	if( ctx->total[0] < (uint32_t) ilen )
		ctx->total[1]++;

	if( left && ilen >= fill )
	{
		memcpy( (void *) (ctx->buffer + left), input, fill );
		ripemd160_process( ctx, ctx->buffer );
		input += fill;
		ilen  -= fill;
		left = 0;
	}

	while( ilen >= _CTB_RIPEMD160_BLOCK_LENGTH )
	{
		ripemd160_process( ctx, input );
		input += _CTB_RIPEMD160_BLOCK_LENGTH;
		ilen  -= _CTB_RIPEMD160_BLOCK_LENGTH;
	}

	if( ilen > 0 )
	{
		memcpy( (void *) (ctx->buffer + left), input, ilen );
	}
}

static const uint8_t ripemd160_padding[_CTB_RIPEMD160_BLOCK_LENGTH] =
	{
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};

/*
 * RIPEMD-160 final digest
 */
void ctb_ripemd160_final( ctb_ripemd160_ctx *ctx, uint8_t output[_CTB_RIPEMD160_DIGEST_LENGTH] )
{
	uint32_t last, padn;
	uint32_t high, low;
	uint8_t msglen[8];

	high = ( ctx->total[0] >> 29 )
		| ( ctx->total[1] <<  3 );
	low  = ( ctx->total[0] <<  3 );

	PUT_UINT32_LE( low,  msglen, 0 );
	PUT_UINT32_LE( high, msglen, 4 );

	last = ctx->total[0] & 0x3F;
	padn = ( last < 56 ) ? ( 56 - last ) : ( 120 - last );

	ctb_ripemd160_update( ctx, ripemd160_padding, padn );
	ctb_ripemd160_update( ctx, msglen, 8 );

	PUT_UINT32_LE( ctx->state[0], output,  0 );
	PUT_UINT32_LE( ctx->state[1], output,  4 );
	PUT_UINT32_LE( ctx->state[2], output,  8 );
	PUT_UINT32_LE( ctx->state[3], output, 12 );
	PUT_UINT32_LE( ctx->state[4], output, 16 );

	ctb__memzero(ctx, sizeof(ctb_ripemd160_ctx));
}

/*
 * output = RIPEMD-160( input buffer )
 */
void ctb_ripemd160(const uint8_t *msg, uint32_t msg_len, uint8_t hash[_CTB_RIPEMD160_DIGEST_LENGTH])
{
	ctb_ripemd160_ctx ctx;
	ctb_ripemd160_init( &ctx );
	ctb_ripemd160_update( &ctx, msg, msg_len );
	ctb_ripemd160_final( &ctx, hash );
}

#undef F1
#undef F2
#undef F3
#undef F4
#undef F5
#undef P
#undef P2

/* =========================================================================
   SHA2 IMPLEMENTATION
   ========================================================================= */
#if 0
#define UNROLL_LOOPS /* Enable loops unrolling */
#endif

#define SHFR(x, n)    (x >> n)
#define ROTR(x, n)   ((x >> n) | (x << ((sizeof(x) << 3) - n)))
#define ROTL(x, n)   ((x << n) | (x >> ((sizeof(x) << 3) - n)))
#define CH(x, y, z)  ((x & y) ^ (~x & z))
#define MAJ(x, y, z) ((x & y) ^ (x & z) ^ (y & z))

#define SHA256_F1(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define SHA256_F2(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define SHA256_F3(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHFR(x,  3))
#define SHA256_F4(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHFR(x, 10))

#define SHA512_F1(x) (ROTR(x, 28) ^ ROTR(x, 34) ^ ROTR(x, 39))
#define SHA512_F2(x) (ROTR(x, 14) ^ ROTR(x, 18) ^ ROTR(x, 41))
#define SHA512_F3(x) (ROTR(x,  1) ^ ROTR(x,  8) ^ SHFR(x,  7))
#define SHA512_F4(x) (ROTR(x, 19) ^ ROTR(x, 61) ^ SHFR(x,  6))

#define UNPACK32(x, str)                      \
{                                             \
	*((str) + 3) = (uint8) ((x)      );       \
	*((str) + 2) = (uint8) ((x) >>  8);       \
	*((str) + 1) = (uint8) ((x) >> 16);       \
	*((str) + 0) = (uint8) ((x) >> 24);       \
}

#define PACK32(str, x)                        \
{                                             \
	*(x) =   ((uint32) *((str) + 3)      )    \
		| ((uint32) *((str) + 2) <<  8)    \
		| ((uint32) *((str) + 1) << 16)    \
		| ((uint32) *((str) + 0) << 24);   \
}

#define UNPACK64(x, str)                      \
{                                             \
	*((str) + 7) = (uint8) ((x)      );       \
	*((str) + 6) = (uint8) ((x) >>  8);       \
	*((str) + 5) = (uint8) ((x) >> 16);       \
	*((str) + 4) = (uint8) ((x) >> 24);       \
	*((str) + 3) = (uint8) ((x) >> 32);       \
	*((str) + 2) = (uint8) ((x) >> 40);       \
	*((str) + 1) = (uint8) ((x) >> 48);       \
	*((str) + 0) = (uint8) ((x) >> 56);       \
}

#define PACK64(str, x)                        \
{                                             \
	*(x) =   ((uint64) *((str) + 7)      )    \
		| ((uint64) *((str) + 6) <<  8)    \
		| ((uint64) *((str) + 5) << 16)    \
		| ((uint64) *((str) + 4) << 24)    \
		| ((uint64) *((str) + 3) << 32)    \
		| ((uint64) *((str) + 2) << 40)    \
		| ((uint64) *((str) + 1) << 48)    \
		| ((uint64) *((str) + 0) << 56);   \
}

/* Macros used for loops unrolling */

#define SHA256_SCR(i)                         \
{                                             \
	w[i] =  SHA256_F4(w[i -  2]) + w[i -  7]  \
		+ SHA256_F3(w[i - 15]) + w[i - 16]; \
}

#define SHA512_SCR(i)                         \
{                                             \
	w[i] =  SHA512_F4(w[i -  2]) + w[i -  7]  \
		+ SHA512_F3(w[i - 15]) + w[i - 16]; \
}

#define SHA256_EXP(a, b, c, d, e, f, g, h, j)               \
{                                                           \
	t1 = wv[h] + SHA256_F2(wv[e]) + CH(wv[e], wv[f], wv[g]) \
		+ sha256_k[j] + w[j];                              \
	t2 = SHA256_F1(wv[a]) + MAJ(wv[a], wv[b], wv[c]);       \
	wv[d] += t1;                                            \
	wv[h] = t1 + t2;                                        \
}

#define SHA512_EXP(a, b, c, d, e, f, g ,h, j)               \
{                                                           \
	t1 = wv[h] + SHA512_F2(wv[e]) + CH(wv[e], wv[f], wv[g]) \
		+ sha512_k[j] + w[j];                              \
	t2 = SHA512_F1(wv[a]) + MAJ(wv[a], wv[b], wv[c]);       \
	wv[d] += t1;                                            \
	wv[h] = t1 + t2;                                        \
}

uint32 sha224_h0[8] =
	{0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
		0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4};

uint32 sha256_h0[8] =
	{0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
		0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

uint64 sha384_h0[8] =
	{0xcbbb9d5dc1059ed8ULL, 0x629a292a367cd507ULL,
		0x9159015a3070dd17ULL, 0x152fecd8f70e5939ULL,
		0x67332667ffc00b31ULL, 0x8eb44a8768581511ULL,
		0xdb0c2e0d64f98fa7ULL, 0x47b5481dbefa4fa4ULL};

uint64 sha512_h0[8] =
	{0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
		0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
		0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
		0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL};

uint32 sha256_k[64] =
	{0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
		0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
		0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
		0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
		0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
		0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
		0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
		0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
		0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
		0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
		0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
		0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
		0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
		0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
		0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
		0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

uint64 sha512_k[80] =
	{0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL,
		0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
		0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
		0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
		0xd807aa98a3030242ULL, 0x12835b0145706fbeULL,
		0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
		0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL,
		0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
		0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
		0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
		0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL,
		0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
		0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL,
		0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
		0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
		0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
		0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL,
		0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
		0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL,
		0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
		0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
		0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
		0xd192e819d6ef5218ULL, 0xd69906245565a910ULL,
		0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
		0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL,
		0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
		0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
		0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
		0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL,
		0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
		0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL,
		0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
		0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
		0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
		0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL,
		0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
		0x28db77f523047d84ULL, 0x32caab7b40c72493ULL,
		0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
		0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
		0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL};

/* SHA-256 functions */

void _ctb_sha256_transf(ctb_sha256_ctx *ctx, const unsigned char *message, unsigned int block_nb)
{
	uint32 w[64];
	uint32 wv[8];
	uint32 t1, t2;
	const unsigned char *sub_block;
	int i;

#ifndef UNROLL_LOOPS
	int j;
#endif

	for (i = 0; i < (int) block_nb; i++) {
		sub_block = message + (i << 6);

#ifndef UNROLL_LOOPS
		for (j = 0; j < 16; j++) {
			PACK32(&sub_block[j << 2], &w[j]);
		}

		for (j = 16; j < 64; j++) {
			SHA256_SCR(j);
		}

		for (j = 0; j < 8; j++) {
			wv[j] = ctx->h[j];
		}

		for (j = 0; j < 64; j++) {
			t1 = wv[7] + SHA256_F2(wv[4]) + CH(wv[4], wv[5], wv[6])
				+ sha256_k[j] + w[j];
			t2 = SHA256_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}

		for (j = 0; j < 8; j++) {
			ctx->h[j] += wv[j];
		}
#else
		PACK32(&sub_block[ 0], &w[ 0]); PACK32(&sub_block[ 4], &w[ 1]);
		PACK32(&sub_block[ 8], &w[ 2]); PACK32(&sub_block[12], &w[ 3]);
		PACK32(&sub_block[16], &w[ 4]); PACK32(&sub_block[20], &w[ 5]);
		PACK32(&sub_block[24], &w[ 6]); PACK32(&sub_block[28], &w[ 7]);
		PACK32(&sub_block[32], &w[ 8]); PACK32(&sub_block[36], &w[ 9]);
		PACK32(&sub_block[40], &w[10]); PACK32(&sub_block[44], &w[11]);
		PACK32(&sub_block[48], &w[12]); PACK32(&sub_block[52], &w[13]);
		PACK32(&sub_block[56], &w[14]); PACK32(&sub_block[60], &w[15]);

		SHA256_SCR(16); SHA256_SCR(17); SHA256_SCR(18); SHA256_SCR(19);
		SHA256_SCR(20); SHA256_SCR(21); SHA256_SCR(22); SHA256_SCR(23);
		SHA256_SCR(24); SHA256_SCR(25); SHA256_SCR(26); SHA256_SCR(27);
		SHA256_SCR(28); SHA256_SCR(29); SHA256_SCR(30); SHA256_SCR(31);
		SHA256_SCR(32); SHA256_SCR(33); SHA256_SCR(34); SHA256_SCR(35);
		SHA256_SCR(36); SHA256_SCR(37); SHA256_SCR(38); SHA256_SCR(39);
		SHA256_SCR(40); SHA256_SCR(41); SHA256_SCR(42); SHA256_SCR(43);
		SHA256_SCR(44); SHA256_SCR(45); SHA256_SCR(46); SHA256_SCR(47);
		SHA256_SCR(48); SHA256_SCR(49); SHA256_SCR(50); SHA256_SCR(51);
		SHA256_SCR(52); SHA256_SCR(53); SHA256_SCR(54); SHA256_SCR(55);
		SHA256_SCR(56); SHA256_SCR(57); SHA256_SCR(58); SHA256_SCR(59);
		SHA256_SCR(60); SHA256_SCR(61); SHA256_SCR(62); SHA256_SCR(63);

		wv[0] = ctx->h[0]; wv[1] = ctx->h[1];
		wv[2] = ctx->h[2]; wv[3] = ctx->h[3];
		wv[4] = ctx->h[4]; wv[5] = ctx->h[5];
		wv[6] = ctx->h[6]; wv[7] = ctx->h[7];

		SHA256_EXP(0,1,2,3,4,5,6,7, 0); SHA256_EXP(7,0,1,2,3,4,5,6, 1);
		SHA256_EXP(6,7,0,1,2,3,4,5, 2); SHA256_EXP(5,6,7,0,1,2,3,4, 3);
		SHA256_EXP(4,5,6,7,0,1,2,3, 4); SHA256_EXP(3,4,5,6,7,0,1,2, 5);
		SHA256_EXP(2,3,4,5,6,7,0,1, 6); SHA256_EXP(1,2,3,4,5,6,7,0, 7);
		SHA256_EXP(0,1,2,3,4,5,6,7, 8); SHA256_EXP(7,0,1,2,3,4,5,6, 9);
		SHA256_EXP(6,7,0,1,2,3,4,5,10); SHA256_EXP(5,6,7,0,1,2,3,4,11);
		SHA256_EXP(4,5,6,7,0,1,2,3,12); SHA256_EXP(3,4,5,6,7,0,1,2,13);
		SHA256_EXP(2,3,4,5,6,7,0,1,14); SHA256_EXP(1,2,3,4,5,6,7,0,15);
		SHA256_EXP(0,1,2,3,4,5,6,7,16); SHA256_EXP(7,0,1,2,3,4,5,6,17);
		SHA256_EXP(6,7,0,1,2,3,4,5,18); SHA256_EXP(5,6,7,0,1,2,3,4,19);
		SHA256_EXP(4,5,6,7,0,1,2,3,20); SHA256_EXP(3,4,5,6,7,0,1,2,21);
		SHA256_EXP(2,3,4,5,6,7,0,1,22); SHA256_EXP(1,2,3,4,5,6,7,0,23);
		SHA256_EXP(0,1,2,3,4,5,6,7,24); SHA256_EXP(7,0,1,2,3,4,5,6,25);
		SHA256_EXP(6,7,0,1,2,3,4,5,26); SHA256_EXP(5,6,7,0,1,2,3,4,27);
		SHA256_EXP(4,5,6,7,0,1,2,3,28); SHA256_EXP(3,4,5,6,7,0,1,2,29);
		SHA256_EXP(2,3,4,5,6,7,0,1,30); SHA256_EXP(1,2,3,4,5,6,7,0,31);
		SHA256_EXP(0,1,2,3,4,5,6,7,32); SHA256_EXP(7,0,1,2,3,4,5,6,33);
		SHA256_EXP(6,7,0,1,2,3,4,5,34); SHA256_EXP(5,6,7,0,1,2,3,4,35);
		SHA256_EXP(4,5,6,7,0,1,2,3,36); SHA256_EXP(3,4,5,6,7,0,1,2,37);
		SHA256_EXP(2,3,4,5,6,7,0,1,38); SHA256_EXP(1,2,3,4,5,6,7,0,39);
		SHA256_EXP(0,1,2,3,4,5,6,7,40); SHA256_EXP(7,0,1,2,3,4,5,6,41);
		SHA256_EXP(6,7,0,1,2,3,4,5,42); SHA256_EXP(5,6,7,0,1,2,3,4,43);
		SHA256_EXP(4,5,6,7,0,1,2,3,44); SHA256_EXP(3,4,5,6,7,0,1,2,45);
		SHA256_EXP(2,3,4,5,6,7,0,1,46); SHA256_EXP(1,2,3,4,5,6,7,0,47);
		SHA256_EXP(0,1,2,3,4,5,6,7,48); SHA256_EXP(7,0,1,2,3,4,5,6,49);
		SHA256_EXP(6,7,0,1,2,3,4,5,50); SHA256_EXP(5,6,7,0,1,2,3,4,51);
		SHA256_EXP(4,5,6,7,0,1,2,3,52); SHA256_EXP(3,4,5,6,7,0,1,2,53);
		SHA256_EXP(2,3,4,5,6,7,0,1,54); SHA256_EXP(1,2,3,4,5,6,7,0,55);
		SHA256_EXP(0,1,2,3,4,5,6,7,56); SHA256_EXP(7,0,1,2,3,4,5,6,57);
		SHA256_EXP(6,7,0,1,2,3,4,5,58); SHA256_EXP(5,6,7,0,1,2,3,4,59);
		SHA256_EXP(4,5,6,7,0,1,2,3,60); SHA256_EXP(3,4,5,6,7,0,1,2,61);
		SHA256_EXP(2,3,4,5,6,7,0,1,62); SHA256_EXP(1,2,3,4,5,6,7,0,63);

		ctx->h[0] += wv[0]; ctx->h[1] += wv[1];
		ctx->h[2] += wv[2]; ctx->h[3] += wv[3];
		ctx->h[4] += wv[4]; ctx->h[5] += wv[5];
		ctx->h[6] += wv[6]; ctx->h[7] += wv[7];
#endif /* !UNROLL_LOOPS */
	}
}

void ctb_sha256(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	ctb_sha256_ctx ctx;

	ctb_sha256_init(&ctx);
	ctb_sha256_update(&ctx, message, len);
	ctb_sha256_final(&ctx, digest);
}

void ctb_sha256_init(ctb_sha256_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->h[i] = sha256_h0[i];
	}
#else
	ctx->h[0] = sha256_h0[0]; ctx->h[1] = sha256_h0[1];
	ctx->h[2] = sha256_h0[2]; ctx->h[3] = sha256_h0[3];
	ctx->h[4] = sha256_h0[4]; ctx->h[5] = sha256_h0[5];
	ctx->h[6] = sha256_h0[6]; ctx->h[7] = sha256_h0[7];
#endif /* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

void ctb_sha256_update(ctb_sha256_ctx *ctx, const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = _CTB_SHA256_BLOCK_SIZE - ctx->len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->block[ctx->len], message, rem_len);

	if (ctx->len + len < _CTB_SHA256_BLOCK_SIZE) {
		ctx->len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / _CTB_SHA256_BLOCK_SIZE;

	shifted_message = message + rem_len;

	_ctb_sha256_transf(ctx, ctx->block, 1);
	_ctb_sha256_transf(ctx, shifted_message, block_nb);

	rem_len = new_len % _CTB_SHA256_BLOCK_SIZE;

	memcpy(ctx->block, &shifted_message[block_nb << 6],
		rem_len);

	ctx->len = rem_len;
	ctx->tot_len += (block_nb + 1) << 6;
}

void ctb_sha256_final(ctb_sha256_ctx *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((_CTB_SHA256_BLOCK_SIZE - 9)
		< (ctx->len % _CTB_SHA256_BLOCK_SIZE)));

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 6;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80;
	UNPACK32(len_b, ctx->block + pm_len - 4);

	_ctb_sha256_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
	for (i = 0 ; i < 8; i++) {
		UNPACK32(ctx->h[i], &digest[i << 2]);
	}
#else
	UNPACK32(ctx->h[0], &digest[ 0]);
	UNPACK32(ctx->h[1], &digest[ 4]);
	UNPACK32(ctx->h[2], &digest[ 8]);
	UNPACK32(ctx->h[3], &digest[12]);
	UNPACK32(ctx->h[4], &digest[16]);
	UNPACK32(ctx->h[5], &digest[20]);
	UNPACK32(ctx->h[6], &digest[24]);
	UNPACK32(ctx->h[7], &digest[28]);
#endif /* !UNROLL_LOOPS */
}

/* SHA-512 functions */

void ctb_sha512_transf(ctb_sha512_ctx *ctx, const unsigned char *message, unsigned int block_nb)
{
	uint64 w[80];
	uint64 wv[8];
	uint64 t1, t2;
	const unsigned char *sub_block;
	int i, j;

	for (i = 0; i < (int) block_nb; i++) {
		sub_block = message + (i << 7);

#ifndef UNROLL_LOOPS
		for (j = 0; j < 16; j++) {
			PACK64(&sub_block[j << 3], &w[j]);
		}

		for (j = 16; j < 80; j++) {
			SHA512_SCR(j);
		}

		for (j = 0; j < 8; j++) {
			wv[j] = ctx->h[j];
		}

		for (j = 0; j < 80; j++) {
			t1 = wv[7] + SHA512_F2(wv[4]) + CH(wv[4], wv[5], wv[6])
				+ sha512_k[j] + w[j];
			t2 = SHA512_F1(wv[0]) + MAJ(wv[0], wv[1], wv[2]);
			wv[7] = wv[6];
			wv[6] = wv[5];
			wv[5] = wv[4];
			wv[4] = wv[3] + t1;
			wv[3] = wv[2];
			wv[2] = wv[1];
			wv[1] = wv[0];
			wv[0] = t1 + t2;
		}

		for (j = 0; j < 8; j++) {
			ctx->h[j] += wv[j];
		}
#else
		PACK64(&sub_block[  0], &w[ 0]); PACK64(&sub_block[  8], &w[ 1]);
		PACK64(&sub_block[ 16], &w[ 2]); PACK64(&sub_block[ 24], &w[ 3]);
		PACK64(&sub_block[ 32], &w[ 4]); PACK64(&sub_block[ 40], &w[ 5]);
		PACK64(&sub_block[ 48], &w[ 6]); PACK64(&sub_block[ 56], &w[ 7]);
		PACK64(&sub_block[ 64], &w[ 8]); PACK64(&sub_block[ 72], &w[ 9]);
		PACK64(&sub_block[ 80], &w[10]); PACK64(&sub_block[ 88], &w[11]);
		PACK64(&sub_block[ 96], &w[12]); PACK64(&sub_block[104], &w[13]);
		PACK64(&sub_block[112], &w[14]); PACK64(&sub_block[120], &w[15]);

		SHA512_SCR(16); SHA512_SCR(17); SHA512_SCR(18); SHA512_SCR(19);
		SHA512_SCR(20); SHA512_SCR(21); SHA512_SCR(22); SHA512_SCR(23);
		SHA512_SCR(24); SHA512_SCR(25); SHA512_SCR(26); SHA512_SCR(27);
		SHA512_SCR(28); SHA512_SCR(29); SHA512_SCR(30); SHA512_SCR(31);
		SHA512_SCR(32); SHA512_SCR(33); SHA512_SCR(34); SHA512_SCR(35);
		SHA512_SCR(36); SHA512_SCR(37); SHA512_SCR(38); SHA512_SCR(39);
		SHA512_SCR(40); SHA512_SCR(41); SHA512_SCR(42); SHA512_SCR(43);
		SHA512_SCR(44); SHA512_SCR(45); SHA512_SCR(46); SHA512_SCR(47);
		SHA512_SCR(48); SHA512_SCR(49); SHA512_SCR(50); SHA512_SCR(51);
		SHA512_SCR(52); SHA512_SCR(53); SHA512_SCR(54); SHA512_SCR(55);
		SHA512_SCR(56); SHA512_SCR(57); SHA512_SCR(58); SHA512_SCR(59);
		SHA512_SCR(60); SHA512_SCR(61); SHA512_SCR(62); SHA512_SCR(63);
		SHA512_SCR(64); SHA512_SCR(65); SHA512_SCR(66); SHA512_SCR(67);
		SHA512_SCR(68); SHA512_SCR(69); SHA512_SCR(70); SHA512_SCR(71);
		SHA512_SCR(72); SHA512_SCR(73); SHA512_SCR(74); SHA512_SCR(75);
		SHA512_SCR(76); SHA512_SCR(77); SHA512_SCR(78); SHA512_SCR(79);

		wv[0] = ctx->h[0]; wv[1] = ctx->h[1];
		wv[2] = ctx->h[2]; wv[3] = ctx->h[3];
		wv[4] = ctx->h[4]; wv[5] = ctx->h[5];
		wv[6] = ctx->h[6]; wv[7] = ctx->h[7];

		j = 0;

		do {
			SHA512_EXP(0,1,2,3,4,5,6,7,j); j++;
			SHA512_EXP(7,0,1,2,3,4,5,6,j); j++;
			SHA512_EXP(6,7,0,1,2,3,4,5,j); j++;
			SHA512_EXP(5,6,7,0,1,2,3,4,j); j++;
			SHA512_EXP(4,5,6,7,0,1,2,3,j); j++;
			SHA512_EXP(3,4,5,6,7,0,1,2,j); j++;
			SHA512_EXP(2,3,4,5,6,7,0,1,j); j++;
			SHA512_EXP(1,2,3,4,5,6,7,0,j); j++;
		} while (j < 80);

		ctx->h[0] += wv[0]; ctx->h[1] += wv[1];
		ctx->h[2] += wv[2]; ctx->h[3] += wv[3];
		ctx->h[4] += wv[4]; ctx->h[5] += wv[5];
		ctx->h[6] += wv[6]; ctx->h[7] += wv[7];
#endif /* !UNROLL_LOOPS */
	}
}

void ctb_sha512(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	ctb_sha512_ctx ctx;

	ctb_sha512_init(&ctx);
	ctb_sha512_update(&ctx, message, len);
	ctb_sha512_final(&ctx, digest);
}

void ctb_sha512_init(ctb_sha512_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->h[i] = sha512_h0[i];
	}
#else
	ctx->h[0] = sha512_h0[0]; ctx->h[1] = sha512_h0[1];
	ctx->h[2] = sha512_h0[2]; ctx->h[3] = sha512_h0[3];
	ctx->h[4] = sha512_h0[4]; ctx->h[5] = sha512_h0[5];
	ctx->h[6] = sha512_h0[6]; ctx->h[7] = sha512_h0[7];
#endif /* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

void ctb_sha512_update(ctb_sha512_ctx *ctx, const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = _CTB_SHA512_BLOCK_SIZE - ctx->len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->block[ctx->len], message, rem_len);

	if (ctx->len + len < _CTB_SHA512_BLOCK_SIZE) {
		ctx->len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / _CTB_SHA512_BLOCK_SIZE;

	shifted_message = message + rem_len;

	ctb_sha512_transf(ctx, ctx->block, 1);
	ctb_sha512_transf(ctx, shifted_message, block_nb);

	rem_len = new_len % _CTB_SHA512_BLOCK_SIZE;

	memcpy(ctx->block, &shifted_message[block_nb << 7],
		rem_len);

	ctx->len = rem_len;
	ctx->tot_len += (block_nb + 1) << 7;
}

void ctb_sha512_final(ctb_sha512_ctx *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	block_nb = 1 + ((_CTB_SHA512_BLOCK_SIZE - 17)
		< (ctx->len % _CTB_SHA512_BLOCK_SIZE));

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 7;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80;
	UNPACK32(len_b, ctx->block + pm_len - 4);

	ctb_sha512_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
	for (i = 0 ; i < 8; i++) {
		UNPACK64(ctx->h[i], &digest[i << 3]);
	}
#else
	UNPACK64(ctx->h[0], &digest[ 0]);
	UNPACK64(ctx->h[1], &digest[ 8]);
	UNPACK64(ctx->h[2], &digest[16]);
	UNPACK64(ctx->h[3], &digest[24]);
	UNPACK64(ctx->h[4], &digest[32]);
	UNPACK64(ctx->h[5], &digest[40]);
	UNPACK64(ctx->h[6], &digest[48]);
	UNPACK64(ctx->h[7], &digest[56]);
#endif /* !UNROLL_LOOPS */
}

/* SHA-384 functions */

void ctb_sha384(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	ctb_sha384_ctx ctx;

	ctb_sha384_init(&ctx);
	ctb_sha384_update(&ctx, message, len);
	ctb_sha384_final(&ctx, digest);
}

void ctb_sha384_init(ctb_sha384_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->h[i] = sha384_h0[i];
	}
#else
	ctx->h[0] = sha384_h0[0]; ctx->h[1] = sha384_h0[1];
	ctx->h[2] = sha384_h0[2]; ctx->h[3] = sha384_h0[3];
	ctx->h[4] = sha384_h0[4]; ctx->h[5] = sha384_h0[5];
	ctx->h[6] = sha384_h0[6]; ctx->h[7] = sha384_h0[7];
#endif /* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

void ctb_sha384_update(ctb_sha384_ctx *ctx, const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = _CTB_SHA384_BLOCK_SIZE - ctx->len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->block[ctx->len], message, rem_len);

	if (ctx->len + len < _CTB_SHA384_BLOCK_SIZE) {
		ctx->len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / _CTB_SHA384_BLOCK_SIZE;

	shifted_message = message + rem_len;

	ctb_sha512_transf(ctx, ctx->block, 1);
	ctb_sha512_transf(ctx, shifted_message, block_nb);

	rem_len = new_len % _CTB_SHA384_BLOCK_SIZE;

	memcpy(ctx->block, &shifted_message[block_nb << 7],
		rem_len);

	ctx->len = rem_len;
	ctx->tot_len += (block_nb + 1) << 7;
}

void ctb_sha384_final(ctb_sha384_ctx *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((_CTB_SHA384_BLOCK_SIZE - 17)
		< (ctx->len % _CTB_SHA384_BLOCK_SIZE)));

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 7;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80;
	UNPACK32(len_b, ctx->block + pm_len - 4);

	ctb_sha512_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
	for (i = 0 ; i < 6; i++) {
		UNPACK64(ctx->h[i], &digest[i << 3]);
	}
#else
	UNPACK64(ctx->h[0], &digest[ 0]);
	UNPACK64(ctx->h[1], &digest[ 8]);
	UNPACK64(ctx->h[2], &digest[16]);
	UNPACK64(ctx->h[3], &digest[24]);
	UNPACK64(ctx->h[4], &digest[32]);
	UNPACK64(ctx->h[5], &digest[40]);
#endif /* !UNROLL_LOOPS */
}

/* SHA-224 functions */

void ctb_sha224(const unsigned char *message, unsigned int len, unsigned char *digest)
{
	ctb_sha224_ctx ctx;

	ctb_sha224_init(&ctx);
	ctb_sha224_update(&ctx, message, len);
	ctb_sha224_final(&ctx, digest);
}

void ctb_sha224_init(ctb_sha224_ctx *ctx)
{
#ifndef UNROLL_LOOPS
	int i;
	for (i = 0; i < 8; i++) {
		ctx->h[i] = sha224_h0[i];
	}
#else
	ctx->h[0] = sha224_h0[0]; ctx->h[1] = sha224_h0[1];
	ctx->h[2] = sha224_h0[2]; ctx->h[3] = sha224_h0[3];
	ctx->h[4] = sha224_h0[4]; ctx->h[5] = sha224_h0[5];
	ctx->h[6] = sha224_h0[6]; ctx->h[7] = sha224_h0[7];
#endif /* !UNROLL_LOOPS */

	ctx->len = 0;
	ctx->tot_len = 0;
}

void ctb_sha224_update(ctb_sha224_ctx *ctx, const unsigned char *message, unsigned int len)
{
	unsigned int block_nb;
	unsigned int new_len, rem_len, tmp_len;
	const unsigned char *shifted_message;

	tmp_len = _CTB_SHA224_BLOCK_SIZE - ctx->len;
	rem_len = len < tmp_len ? len : tmp_len;

	memcpy(&ctx->block[ctx->len], message, rem_len);

	if (ctx->len + len < _CTB_SHA224_BLOCK_SIZE) {
		ctx->len += len;
		return;
	}

	new_len = len - rem_len;
	block_nb = new_len / _CTB_SHA224_BLOCK_SIZE;

	shifted_message = message + rem_len;

	_ctb_sha256_transf(ctx, ctx->block, 1);
	_ctb_sha256_transf(ctx, shifted_message, block_nb);

	rem_len = new_len % _CTB_SHA224_BLOCK_SIZE;

	memcpy(ctx->block, &shifted_message[block_nb << 6],
		rem_len);

	ctx->len = rem_len;
	ctx->tot_len += (block_nb + 1) << 6;
}

void ctb_sha224_final(ctb_sha224_ctx *ctx, unsigned char *digest)
{
	unsigned int block_nb;
	unsigned int pm_len;
	unsigned int len_b;

#ifndef UNROLL_LOOPS
	int i;
#endif

	block_nb = (1 + ((_CTB_SHA224_BLOCK_SIZE - 9)
		< (ctx->len % _CTB_SHA224_BLOCK_SIZE)));

	len_b = (ctx->tot_len + ctx->len) << 3;
	pm_len = block_nb << 6;

	memset(ctx->block + ctx->len, 0, pm_len - ctx->len);
	ctx->block[ctx->len] = 0x80;
	UNPACK32(len_b, ctx->block + pm_len - 4);

	_ctb_sha256_transf(ctx, ctx->block, block_nb);

#ifndef UNROLL_LOOPS
	for (i = 0 ; i < 7; i++) {
		UNPACK32(ctx->h[i], &digest[i << 2]);
	}
#else
	UNPACK32(ctx->h[0], &digest[ 0]);
	UNPACK32(ctx->h[1], &digest[ 4]);
	UNPACK32(ctx->h[2], &digest[ 8]);
	UNPACK32(ctx->h[3], &digest[12]);
	UNPACK32(ctx->h[4], &digest[16]);
	UNPACK32(ctx->h[5], &digest[20]);
	UNPACK32(ctx->h[6], &digest[24]);
#endif /* !UNROLL_LOOPS */
}

#undef ROTR
#undef ROTL
#undef CH
#undef MAJ
#undef SHFR

/* =========================================================================
   HMAC IMPLEMENTATION
   ========================================================================= */

void ctb_hmac_sha224_init(ctb_hmac_sha224_ctx *ctx, const unsigned char *key,
						  unsigned int key_size)
{
	unsigned int fill;
	unsigned int num;

	const unsigned char *key_used;
	unsigned char key_temp[_CTB_SHA224_DIGEST_SIZE];
	int i;

	if (key_size == _CTB_SHA224_BLOCK_SIZE) {
		key_used = key;
		num = _CTB_SHA224_BLOCK_SIZE;
	} else {
		if (key_size > _CTB_SHA224_BLOCK_SIZE){
			num = _CTB_SHA224_DIGEST_SIZE;
			ctb_sha224(key, key_size, key_temp);
			key_used = key_temp;
		} else { /* key_size > _CTB_SHA224_BLOCK_SIZE */
			key_used = key;
			num = key_size;
		}
		fill = _CTB_SHA224_BLOCK_SIZE - num;

		memset(ctx->block_ipad + num, 0x36, fill);
		memset(ctx->block_opad + num, 0x5c, fill);
	}

	for (i = 0; i < (int) num; i++) {
		ctx->block_ipad[i] = key_used[i] ^ 0x36;
		ctx->block_opad[i] = key_used[i] ^ 0x5c;
	}

	ctb_sha224_init(&ctx->ctx_inside);
	ctb_sha224_update(&ctx->ctx_inside, ctx->block_ipad, _CTB_SHA224_BLOCK_SIZE);

	ctb_sha224_init(&ctx->ctx_outside);
	ctb_sha224_update(&ctx->ctx_outside, ctx->block_opad,
				   _CTB_SHA224_BLOCK_SIZE);

	/* for hmac_reinit */
	memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
		sizeof(ctb_sha224_ctx));
	memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
		sizeof(ctb_sha224_ctx));
}

void ctb_hmac_sha224_reinit(ctb_hmac_sha224_ctx *ctx)
{
	memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
		sizeof(ctb_sha224_ctx));
	memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
		sizeof(ctb_sha224_ctx));
}

void ctb_hmac_sha224_update(ctb_hmac_sha224_ctx *ctx, const unsigned char *message,
							unsigned int message_len)
{
	ctb_sha224_update(&ctx->ctx_inside, message, message_len);
}

void ctb_hmac_sha224_final(ctb_hmac_sha224_ctx *ctx, unsigned char *mac,
						   unsigned int mac_size)
{
	unsigned char digest_inside[_CTB_SHA224_DIGEST_SIZE];
	unsigned char mac_temp[_CTB_SHA224_DIGEST_SIZE];

	ctb_sha224_final(&ctx->ctx_inside, digest_inside);
	ctb_sha224_update(&ctx->ctx_outside, digest_inside, _CTB_SHA224_DIGEST_SIZE);
	ctb_sha224_final(&ctx->ctx_outside, mac_temp);
	memcpy(mac, mac_temp, mac_size);
}

void ctb_hmac_sha224(const unsigned char *key, unsigned int key_size,
					 const unsigned char *message, unsigned int message_len,
					 unsigned char *mac, unsigned mac_size)
{
	ctb_hmac_sha224_ctx ctx;

	ctb_hmac_sha224_init(&ctx, key, key_size);
	ctb_hmac_sha224_update(&ctx, message, message_len);
	ctb_hmac_sha224_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-256 functions */

void ctb_hmac_sha256_init(ctb_hmac_sha256_ctx *ctx, const unsigned char *key,
						  unsigned int key_size)
{
	unsigned int fill;
	unsigned int num;

	const unsigned char *key_used;
	unsigned char key_temp[_CTB_SHA256_DIGEST_SIZE];
	int i;

	if (key_size == _CTB_SHA256_BLOCK_SIZE) {
		key_used = key;
		num = _CTB_SHA256_BLOCK_SIZE;
	} else {
		if (key_size > _CTB_SHA256_BLOCK_SIZE){
			num = _CTB_SHA256_DIGEST_SIZE;
			ctb_sha256(key, key_size, key_temp);
			key_used = key_temp;
		} else { /* key_size > _CTB_SHA256_BLOCK_SIZE */
			key_used = key;
			num = key_size;
		}
		fill = _CTB_SHA256_BLOCK_SIZE - num;

		memset(ctx->block_ipad + num, 0x36, fill);
		memset(ctx->block_opad + num, 0x5c, fill);
	}

	for (i = 0; i < (int) num; i++) {
		ctx->block_ipad[i] = key_used[i] ^ 0x36;
		ctx->block_opad[i] = key_used[i] ^ 0x5c;
	}

	ctb_sha256_init(&ctx->ctx_inside);
	ctb_sha256_update(&ctx->ctx_inside, ctx->block_ipad, _CTB_SHA256_BLOCK_SIZE);

	ctb_sha256_init(&ctx->ctx_outside);
	ctb_sha256_update(&ctx->ctx_outside, ctx->block_opad,
				   _CTB_SHA256_BLOCK_SIZE);

	/* for hmac_reinit */
	memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
		sizeof(ctb_sha256_ctx));
	memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
		sizeof(ctb_sha256_ctx));
}

void ctb_hmac_sha256_reinit(ctb_hmac_sha256_ctx *ctx)
{
	memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
		sizeof(ctb_sha256_ctx));
	memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
		sizeof(ctb_sha256_ctx));
}

void ctb_hmac_sha256_update(ctb_hmac_sha256_ctx *ctx, const unsigned char *message,
							unsigned int message_len)
{
	ctb_sha256_update(&ctx->ctx_inside, message, message_len);
}

void ctb_hmac_sha256_final(ctb_hmac_sha256_ctx *ctx, unsigned char *mac,
						   unsigned int mac_size)
{
	unsigned char digest_inside[_CTB_SHA256_DIGEST_SIZE];
	unsigned char mac_temp[_CTB_SHA256_DIGEST_SIZE];

	ctb_sha256_final(&ctx->ctx_inside, digest_inside);
	ctb_sha256_update(&ctx->ctx_outside, digest_inside, _CTB_SHA256_DIGEST_SIZE);
	ctb_sha256_final(&ctx->ctx_outside, mac_temp);
	memcpy(mac, mac_temp, mac_size);
}

void ctb_hmac_sha256(const unsigned char *key, unsigned int key_size,
					 const unsigned char *message, unsigned int message_len,
					 unsigned char *mac, unsigned mac_size)
{
	ctb_hmac_sha256_ctx ctx;

	ctb_hmac_sha256_init(&ctx, key, key_size);
	ctb_hmac_sha256_update(&ctx, message, message_len);
	ctb_hmac_sha256_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-384 functions */

void ctb_hmac_sha384_init(ctb_hmac_sha384_ctx *ctx, const unsigned char *key,
						  unsigned int key_size)
{
	unsigned int fill;
	unsigned int num;

	const unsigned char *key_used;
	unsigned char key_temp[_CTB_SHA384_DIGEST_SIZE];
	int i;

	if (key_size == _CTB_SHA384_BLOCK_SIZE) {
		key_used = key;
		num = _CTB_SHA384_BLOCK_SIZE;
	} else {
		if (key_size > _CTB_SHA384_BLOCK_SIZE){
			num = _CTB_SHA384_DIGEST_SIZE;
			ctb_sha384(key, key_size, key_temp);
			key_used = key_temp;
		} else { /* key_size > _CTB_SHA384_BLOCK_SIZE */
			key_used = key;
			num = key_size;
		}
		fill = _CTB_SHA384_BLOCK_SIZE - num;

		memset(ctx->block_ipad + num, 0x36, fill);
		memset(ctx->block_opad + num, 0x5c, fill);
	}

	for (i = 0; i < (int) num; i++) {
		ctx->block_ipad[i] = key_used[i] ^ 0x36;
		ctx->block_opad[i] = key_used[i] ^ 0x5c;
	}

	ctb_sha384_init(&ctx->ctx_inside);
	ctb_sha384_update(&ctx->ctx_inside, ctx->block_ipad, _CTB_SHA384_BLOCK_SIZE);

	ctb_sha384_init(&ctx->ctx_outside);
	ctb_sha384_update(&ctx->ctx_outside, ctx->block_opad,
				   _CTB_SHA384_BLOCK_SIZE);

	/* for hmac_reinit */
	memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
		sizeof(ctb_sha384_ctx));
	memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
		sizeof(ctb_sha384_ctx));
}

void ctb_hmac_sha384_reinit(ctb_hmac_sha384_ctx *ctx)
{
	memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
		sizeof(ctb_sha384_ctx));
	memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
		sizeof(ctb_sha384_ctx));
}

void ctb_hmac_sha384_update(ctb_hmac_sha384_ctx *ctx, const unsigned char *message,
							unsigned int message_len)
{
	ctb_sha384_update(&ctx->ctx_inside, message, message_len);
}

void ctb_hmac_sha384_final(ctb_hmac_sha384_ctx *ctx, unsigned char *mac,
						   unsigned int mac_size)
{
	unsigned char digest_inside[_CTB_SHA384_DIGEST_SIZE];
	unsigned char mac_temp[_CTB_SHA384_DIGEST_SIZE];

	ctb_sha384_final(&ctx->ctx_inside, digest_inside);
	ctb_sha384_update(&ctx->ctx_outside, digest_inside, _CTB_SHA384_DIGEST_SIZE);
	ctb_sha384_final(&ctx->ctx_outside, mac_temp);
	memcpy(mac, mac_temp, mac_size);
}

void ctb_hmac_sha384(const unsigned char *key, unsigned int key_size,
					 const unsigned char *message, unsigned int message_len,
					 unsigned char *mac, unsigned mac_size)
{
	ctb_hmac_sha384_ctx ctx;

	ctb_hmac_sha384_init(&ctx, key, key_size);
	ctb_hmac_sha384_update(&ctx, message, message_len);
	ctb_hmac_sha384_final(&ctx, mac, mac_size);
}

/* HMAC-SHA-512 functions */

void ctb_hmac_sha512_init(ctb_hmac_sha512_ctx *ctx, const unsigned char *key,
						  unsigned int key_size)
{
	unsigned int fill;
	unsigned int num;

	const unsigned char *key_used;
	unsigned char key_temp[_CTB_SHA512_DIGEST_SIZE];
	int i;

	if (key_size == _CTB_SHA512_BLOCK_SIZE) {
		key_used = key;
		num = _CTB_SHA512_BLOCK_SIZE;
	} else {
		if (key_size > _CTB_SHA512_BLOCK_SIZE){
			num = _CTB_SHA512_DIGEST_SIZE;
			ctb_sha512(key, key_size, key_temp);
			key_used = key_temp;
		} else { /* key_size > _CTB_SHA512_BLOCK_SIZE */
			key_used = key;
			num = key_size;
		}
		fill = _CTB_SHA512_BLOCK_SIZE - num;

		memset(ctx->block_ipad + num, 0x36, fill);
		memset(ctx->block_opad + num, 0x5c, fill);
	}

	for (i = 0; i < (int) num; i++) {
		ctx->block_ipad[i] = key_used[i] ^ 0x36;
		ctx->block_opad[i] = key_used[i] ^ 0x5c;
	}

	ctb_sha512_init(&ctx->ctx_inside);
	ctb_sha512_update(&ctx->ctx_inside, ctx->block_ipad, _CTB_SHA512_BLOCK_SIZE);

	ctb_sha512_init(&ctx->ctx_outside);
	ctb_sha512_update(&ctx->ctx_outside, ctx->block_opad,
				   _CTB_SHA512_BLOCK_SIZE);

	/* for hmac_reinit */
	memcpy(&ctx->ctx_inside_reinit, &ctx->ctx_inside,
		sizeof(ctb_sha512_ctx));
	memcpy(&ctx->ctx_outside_reinit, &ctx->ctx_outside,
		sizeof(ctb_sha512_ctx));
}

void ctb_hmac_sha512_reinit(ctb_hmac_sha512_ctx *ctx)
{
	memcpy(&ctx->ctx_inside, &ctx->ctx_inside_reinit,
		sizeof(ctb_sha512_ctx));
	memcpy(&ctx->ctx_outside, &ctx->ctx_outside_reinit,
		sizeof(ctb_sha512_ctx));
}

void ctb_hmac_sha512_update(ctb_hmac_sha512_ctx *ctx, const unsigned char *message,
							unsigned int message_len)
{
	ctb_sha512_update(&ctx->ctx_inside, message, message_len);
}

void ctb_hmac_sha512_final(ctb_hmac_sha512_ctx *ctx, unsigned char *mac,
						   unsigned int mac_size)
{
	unsigned char digest_inside[_CTB_SHA512_DIGEST_SIZE];
	unsigned char mac_temp[_CTB_SHA512_DIGEST_SIZE];

	ctb_sha512_final(&ctx->ctx_inside, digest_inside);
	ctb_sha512_update(&ctx->ctx_outside, digest_inside, _CTB_SHA512_DIGEST_SIZE);
	ctb_sha512_final(&ctx->ctx_outside, mac_temp);
	memcpy(mac, mac_temp, mac_size);
}

void ctb_hmac_sha512(const unsigned char *key, unsigned int key_size,
					 const unsigned char *message, unsigned int message_len,
					 unsigned char *mac, unsigned mac_size)
{
	ctb_hmac_sha512_ctx ctx;

	ctb_hmac_sha512_init(&ctx, key, key_size);
	ctb_hmac_sha512_update(&ctx, message, message_len);
	ctb_hmac_sha512_final(&ctx, mac, mac_size);
}


/* =========================================================================
   PBKDF2 IMPLEMENTATION
   ========================================================================= */

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
	memcpy(out + off, T, take);											\
	off += take;                                    \
}                                                   \
\
/* wipe & free */                                   \
if (U) { memset(U, 0, hLen); free(U); }             \
if (T) { memset(T, 0, hLen); free(T); }             \
if (salt_cnt) { memset(salt_cnt, 0, salt_len + 4); free(salt_cnt); } \
}

/* Instantiate four PBKDF2 functions (HMAC-SHA2 variants) */
DECL_PBKDF2_FN(ctb_pbkdf2_hmac_sha224,
			   ctb_hmac_sha224_ctx,
			   ctb_hmac_sha224_init, ctb_hmac_sha224_reinit,
			   ctb_hmac_sha224_update, ctb_hmac_sha224_final,
			   _CTB_SHA224_DIGEST_SIZE)

DECL_PBKDF2_FN(ctb_pbkdf2_hmac_sha256,
			   ctb_hmac_sha256_ctx,
			   ctb_hmac_sha256_init, ctb_hmac_sha256_reinit,
			   ctb_hmac_sha256_update, ctb_hmac_sha256_final,
			   _CTB_SHA256_DIGEST_SIZE)

DECL_PBKDF2_FN(ctb_pbkdf2_hmac_sha384,
			   ctb_hmac_sha384_ctx,
			   ctb_hmac_sha384_init, ctb_hmac_sha384_reinit,
			   ctb_hmac_sha384_update, ctb_hmac_sha384_final,
			   _CTB_SHA384_DIGEST_SIZE)

DECL_PBKDF2_FN(ctb_pbkdf2_hmac_sha512,
			   ctb_hmac_sha512_ctx,
			   ctb_hmac_sha512_init, ctb_hmac_sha512_reinit,
			   ctb_hmac_sha512_update, ctb_hmac_sha512_final,
			   _CTB_SHA512_DIGEST_SIZE)


#endif /* CTB_HASH_IMPLEMENTATION */
