#ifndef _CTB_H
#define _CTB_H

#ifdef CTB_NOPREFIX
	#define CTB_STRING_NOPREFIX
	#define CTB_ARENA_NOPREFIX
#endif

#ifdef CTB_IMPLEMENTATION
	#define CTB_STRING_IMPLEMENTATION
	#define CTB_ARENA_IMPLEMENTATION
#endif

#include "ctb_types.h"
#include "ctb_arena.h"
#include "ctb_string.h"

#endif
