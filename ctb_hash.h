#ifndef _CTB_HASH_H
#define _CTB_HASH_H


#ifdef CTB_HASH_NOPREFIX
	#define CTB_RIPEMD160_NOPREFIX
	#define CTB_SHA1_NOPREFIX
	#define CTB_SHA2_NOPREFIX
	#define CTB_HMAC_SHA2_NOPREFIX
#endif

#ifdef CTB_HASH_IMPLEMENTATION
	#define CTB_RIPEMD160_IMPLEMENTATION
	#define CTB_SHA1_IMPLEMENTATION
	#define CTB_SHA2_IMPLEMENTATION
	#define CTB_HMAC_SHA2_IMPLEMENTATION
#endif


#include "ctb_ripemd160.h"
#include "ctb_sha1.h"
#include "ctb_sha2.h"
#include "ctb_hmac_sha2.h"

#endif
