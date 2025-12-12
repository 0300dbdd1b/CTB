#ifndef _CTB_H
#define _CTB_H

#ifdef CTB_NOPREFIX
	#define CTB_PLATFORM_NOPREFIX
	#define CTB_MACROS_NOPREFIX
	#define CTB_TYPES_NOPREFIX
	#define CTB_MATRIX_NOPREFIX
	#define CTB_STRING_NOPREFIX
	#define CTB_ARENA_NOPREFIX
	#define CTB_DA_NOPREFIX
	#define CTB_COLORS_NOPREFIX
	#define CTB_LOG_NOPREFIX
	#define CTB_HASH_NOPREFIX
#endif

#ifdef CTB_IMPLEMENTATION
	#define CTB_PLATFORM_IMPLEMENTATION
	#define CTB_MACROS_IMPLEMENTATION
	#define CTB_TYPES_IMPLEMENTATION
	#define CTB_MATRIX_IMPLEMENTATION
	#define CTB_STRING_IMPLEMENTATION
	#define CTB_ARENA_IMPLEMENTATION
	#define CTB_DA_IMPLEMENTATION
	#define CTB_COLORS_IMPLEMENTATION
	#define CTB_LOG_IMPLEMENTATION
	#define CTB_HASH_IMPLEMENTATION
#endif


#include "ctb_platform.h"
#include "ctb_macros.h"
#include "ctb_types.h"
#include "ctb_string.h"
#include "ctb_arena.h"
#include "ctb_da.h"
#include "ctb_colors.h"
#include "ctb_log.h"
#include "ctb_hash.h"

#endif
