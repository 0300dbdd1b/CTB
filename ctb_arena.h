#ifndef _CTB_ARENA_H
#define _CTB_ARENA_H


#ifdef __cplusplus extern "C" {
#endif

#ifndef CTB_INTERNALS
#define CTB_INTERNALS

#if defined(__STDC__)
	#include <stdint.h>
	typedef	uint8_t		U8;
	typedef	uint16_t	U16;
	typedef	uint32_t	U32;
	typedef	uint64_t	U64;
	typedef	int8_t		S8;
	typedef	int16_t		S16;
	typedef	int32_t		S32;
	typedef	int64_t		S64;
	typedef	float		F32;
	typedef	double		F64;
	typedef	uintptr_t	UPTR;
	typedef	intptr_t	IPTR;
#else
	typedef unsigned char		U8;
	typedef unsigned short		U16;
	typedef unsigned int		U32;
	typedef unsigned long long	U64;
	typedef signed char			S8;
	typedef signed short		S16;
	typedef signed int			S32;
	typedef signed long long	S64;
	typedef float				F32;
	typedef double				F64;
	typedef	unsigned long		UPTR;
	typedef	long				IPTR;
#endif

#define CTB_IS_BIT_SET(value, bit)	(((value) >> (bit)) & 1)
#define CTB_SET_BIT(value, bit)		((value) |= (1ULL << (bit)))
#define CTB_CLEAR_BIT(value, bit)	((value) &= ~(1ULL << (bit)))

#if defined(CTB_DEBUGPRINT)
	#if defined(__STDC__)
		#include <stdio.h>
		#define CTB_INTERNALS_DEBUGPRINT(...) printf(__VA_ARGS__)
	#else
		#define CTB_INTERNALS_DEBUGPRINT(...) ((void)0)
	#endif
#else
	#define CTB_INTERNALS_DEBUGPRINT(...) ((void)0)
#endif

#endif // NOTE: !CTB_INTERNALS


#if defined(CTB_ARENA_STATIC)
	#define	CTB_PUBLICDEC static
	#define CTB_PUBLICDEF static
#elif defined(__cplusplus)
	#define CTB_PUBLICDEC extern "C"
	#define	CTB_PUBLICDEF extern "C"
#else
	#define CTB_PUBLICDEC extern
	#define CTB_PUBLICDEF
#endif


typedef enum ctb_arena_flags
{
	NONE				= 0,
	MEMSET_ON_RESET		= 1 << 0,
	USE_HEADER			= 1 << 1
} ctb_arena_flags;


typedef struct ctb_arena ctb_arena;
struct ctb_arena
{
	ctb_arena_flags	flags;
	ctb_arena		*prev;
	ctb_arena		*next;
	U8				*memory;
	U64				capacity;
	U64				offset;
};

typedef struct
{
	U64	size;
} ctb_arena_allocation_header;


CTB_PUBLICDEC	void *ctb_arena_alloc_aligned(ctb_arena *arena, U64 size, U64 alignment);
CTB_PUBLICDEC	void *ctb_arena_alloc(ctb_arena *arena, U64 size);
CTB_PUBLICDEC	void *ctb_arena_realloc(ctb_arena *arena, void *old_ptr, U64 new_size);
CTB_PUBLICDEC	ctb_arena *ctb_arena_create(U64 capacity);
CTB_PUBLICDEC	ctb_arena *ctb_arena_extend(ctb_arena *arena, U64 size);
CTB_PUBLICDEC	ctb_arena *ctb_arena_reset(ctb_arena *arena);
CTB_PUBLICDEC	void ctb_arena_destroy(ctb_arena *arena);
CTB_PUBLICDEC	ctb_arena *ctb_arena_copy(const ctb_arena *source);
CTB_PUBLICDEC	char *ctb_arena_strdup(ctb_arena *arena, const char *cstr);

CTB_PUBLICDEC	void *ctb_arena_internal_memcpy(void *dest, const void *src, U64 size);
CTB_PUBLICDEC	void *ctb_arena_internal_memset(void *dest, U8 value, U64 size);
CTB_PUBLICDEC	U64 ctb_arena_internal_strlen(const char *cstr);


#ifdef __cplusplus
}
#endif
#endif	// NOTE: !_CTB_ARENA_H

/*****************************************************************************/

#ifdef CTB_ARENA_IMPLEMENTATION

#ifndef CTB_ARENA_DEFAULT_ALIGNMENT
	#define CTB_DEFAULT_ALIGNMENT (sizeof(void*))
#endif

#ifndef CTB_ARENA_DEFAULT_FLAGS
	#define CTB_ARENA_DEFAULT_FLAGS (0 | USE_HEADER)
#endif

#ifndef CTB_ARENA_ALLOCATOR
	#if defined(__STDC__)
		#include <stdlib.h>
		#define CTB_ARENA_ALLOCATOR malloc
	#endif
#endif

#if defined(__STDC__)
	#include <stdlib.h>
	#include <string.h>
#endif


CTB_PUBLICDEF
ctb_arena	*ctb_arena_create(U64 capacity)
{
	ctb_arena	*arena = CTB_ARENA_ALLOCATOR(sizeof(ctb_arena));
	if (!arena)
	{
		return 0;
	}
	arena->flags = CTB_ARENA_DEFAULT_FLAGS;
	arena->prev = 0;
	arena->next = 0;
	arena->offset = 0;
	arena->capacity = capacity;
	arena->memory = CTB_ARENA_ALLOCATOR(capacity);
	if (!arena->memory)
	{
		free(arena);
		return 0;
	}
	CTB_INTERNALS_DEBUGPRINT("[ARENA] ctb_arena_create: arena of %llub capacity successfully created at %p\n", capacity, (void*)arena);
	return arena;
}

CTB_PUBLICDEF
ctb_arena *ctb_arena_extend(ctb_arena *arena, U64 size)
{
	if (!arena) return 0;
	ctb_arena	*new_arena	= ctb_arena_create(size);
	if (!new_arena)
	{
		CTB_INTERNALS_DEBUGPRINT("[ARENA] ctb_arena_extend: allocation failed [FAIL]\n");
		return (ctb_arena *)0;
	}
	new_arena->flags = arena->flags;
	while (arena->next)
	{
		arena = arena->next;
	}
	arena->next = new_arena;
	new_arena->prev = arena;
	CTB_INTERNALS_DEBUGPRINT("[ARENA] ctb_arena_extend: arena %p extended, new segment at %p\n", (void*)arena, (void*)new_arena);
	return new_arena;
}


CTB_PUBLICDEF
void *ctb_arena_alloc_aligned(ctb_arena *arena, U64 size, U64 alignment)
{
	if (!arena) return 0;
	U64 header_size = CTB_IS_BIT_SET(arena->flags, USE_HEADER) ? sizeof(ctb_arena_allocation_header) : 0;
	while (arena)
	{
		UPTR current = (UPTR)(arena->memory + arena->offset);
		UPTR aligned = (current + alignment - 1 + header_size) & ~(UPTR)(alignment - 1);
		U64 padding = aligned - current;
		U64 total = size + padding;

		if (arena->offset + total <= arena->capacity)
		{
			void *ptr = arena->memory + arena->offset + padding;
			ctb_arena_allocation_header header = { .size = size };
			memcpy((U8*)ptr - header_size, &header, header_size);
			arena->offset += total;
			CTB_INTERNALS_DEBUGPRINT("[ARENA] ctb_arena_alloc_aligned: %llub allocated (%llub used) in arena %p\n", size, total, (void*)arena);
			return ptr;
		}
		if (!arena->next)
		{
			arena->next = ctb_arena_create(size + arena->capacity);
			if (!arena->next)
			{
				return 0;
			}
			arena->next->prev = arena;
		}
		arena = arena->next;
	}
	return 0;
}

CTB_PUBLICDEF
void *ctb_arena_alloc(ctb_arena *arena, U64 size)
{
	return (ctb_arena_alloc_aligned(arena, size, CTB_DEFAULT_ALIGNMENT));
}

CTB_PUBLICDEF
void *ctb_arena_realloc(ctb_arena *arena, void *old_ptr, U64 new_size)
{
	void *ptr = 0;
	if (CTB_IS_BIT_SET(arena->flags, USE_HEADER))
	{
		ctb_arena_allocation_header header;
		ptr = ctb_arena_alloc(arena, new_size);
		ctb_arena_internal_memcpy(&header, (U8*)old_ptr - sizeof(header), sizeof(header));
		ctb_arena_internal_memcpy(ptr, old_ptr, header.size);
		ctb_arena_internal_memset((U8*)old_ptr - sizeof(header), 0, header.size + sizeof(header));
	}
	return ptr;
}

CTB_PUBLICDEF
ctb_arena	*ctb_arena_reset(ctb_arena *arena)
{
	ctb_arena *it = arena;
	while (it)
	{
		it->offset = 0;
		if (CTB_IS_BIT_SET(arena->flags, MEMSET_ON_RESET))
		{
			ctb_arena_internal_memset(arena->memory, 0, arena->capacity);
		}
		it = it->next;
	}
	return arena;
}

CTB_PUBLICDEF
void ctb_arena_destroy(ctb_arena *arena)
{
	if (!arena) return;
	ctb_arena *it = arena;
	while (it && it->prev)
	{
		it = it->prev;
	}
	while (it)
	{
		ctb_arena *next = it->next;
		free(it->memory);
		it->prev = 0;
		it->next = 0;
		it->capacity = 0;
		it->offset = 0;
		CTB_INTERNALS_DEBUGPRINT("[ARENA] ctb_arena_destroy: Arena at %p destroyed\n", (void*)it);
		free(it);
		it = next;
	}
}

CTB_PUBLICDEF
ctb_arena *ctb_arena_copy(const ctb_arena *source)
{
	if (!source) return 0;
	ctb_arena *copy = ctb_arena_create(source->capacity);
	if (!copy) return 0;

	copy->flags = source->flags;
	copy->offset = source->offset;
	ctb_arena_internal_memcpy(copy->memory, source->memory, source->offset);

	const ctb_arena *src_it = source->next;
	ctb_arena *dst_it = copy;

	while (src_it)
	{
		ctb_arena *new_segment = ctb_arena_create(src_it->capacity);
		if (!new_segment)
		{
			ctb_arena_destroy(copy);
			return 0;
		}
		new_segment->flags = src_it->flags;
		new_segment->offset = src_it->offset;
		ctb_arena_internal_memcpy(new_segment->memory, src_it->memory, src_it->offset);

		dst_it->next = new_segment;
		new_segment->prev = dst_it;

		src_it = src_it->next;
		dst_it = new_segment;
	}

	CTB_INTERNALS_DEBUGPRINT("[ARENA] ctb_arena_copy: copied arena %p -> %p\n", (void*)source, (void*)copy);
	return copy;
}

CTB_PUBLICDEF
U64 ctb_arena_internal_strlen(const char *cstr)
{
	U64 len = 0;
	while (cstr[len])
	{
		len++;
	}
	return len;
}

CTB_PUBLICDEF
void *ctb_arena_internal_memcpy(void *dest, const void *src, U64 size)
{
	for (U64 i = 0; i < size; i++)
	{
		((U8*)(dest))[i] = ((U8*)(src))[i];
	}
	return dest;
}

CTB_PUBLICDEF
void *ctb_arena_internal_memset(void *dest, U8 value, U64 size)
{
	U8 *ptr = dest;
	for (U64 i = 0; i < size; i++)
	{
		ptr[i] = value;
	}
	return dest;
}

CTB_PUBLICDEF
char *ctb_arena_strdup(ctb_arena *arena, const char *cstr)
{
	U64 len = ctb_arena_internal_strlen(cstr);
	char *dup = (char*)ctb_arena_alloc(arena, len + 1);
	ctb_arena_internal_memcpy(dup, cstr, len);
	dup[len] = '\0';
	return dup;
}




#endif	// NOTE: !CTB_ARENA_IMPLEMENTATION


