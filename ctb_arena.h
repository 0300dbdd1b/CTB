#ifndef _CTB_ARENA_H
#define _CTB_ARENA_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================= Configuration =========================
   Define before including:
   - CTB_ARENA_STATIC            : make all functions static
   - CTB_ARENA_DEBUG             : enable debug logs (stderr if libc, else no-op)
   - CTB_ARENA_USE_LIBC          : use malloc/free/memcpy/memset/strlen
   - CTB_ARENA_DEFAULT_ALIGNMENT : defaults to sizeof(void*)
   - CTB_ARENA_DEFAULT_FLAGS     : defaults to (CTB_ARENA_FLAG_USE_HEADER)
   - CTB_ARENA_MALLOC, CTB_ARENA_FREE,
     CTB_ARENA_MEMCPY, CTB_ARENA_MEMSET, CTB_ARENA_STRLEN : custom hooks
   ================================================================= */

#if defined(CTB_ARENA_STATIC)
#	define CTB_ARENA_DEC static
#	define CTB_ARENA_DEF static
#elif defined(__cplusplus)
#	define CTB_ARENA_DEC extern "C"
#	define CTB_ARENA_DEF extern "C"
#else
#	define CTB_ARENA_DEC extern
#	define CTB_ARENA_DEF
#endif

enum ctb_arena_flags
{
	CTB_ARENA_FLAG_NONE				= 0,
	CTB_ARENA_FLAG_MEMSET_ON_SET	= 1u << 0, /* zero memory on arena creation */
	CTB_ARENA_FLAG_MEMSET_ON_RESET	= 1u << 1, /* zero memory on reset */
	CTB_ARENA_FLAG_USE_HEADER		= 1u << 2  /* store size header before each alloc (for realloc/clear) */
};

struct ctb_arena
{
	unsigned int		flags;
	struct ctb_arena*	prev;
	struct ctb_arena*	next;
	unsigned char*		memory;
	unsigned long long	capacity;
	unsigned long long	offset;
};
typedef struct ctb_arena ctb_arena;

struct ctb_arena_allocation_header
{
	unsigned long long	size;
};

CTB_ARENA_DEC void*				ctb_arena_alloc_aligned(struct ctb_arena* arena, unsigned long long size,
														 unsigned long long alignment);
CTB_ARENA_DEC void*				ctb_arena_alloc(struct ctb_arena* arena, unsigned long long size);
CTB_ARENA_DEC void*				ctb_arena_realloc(struct ctb_arena* arena, void* old_ptr, unsigned long long new_size);
CTB_ARENA_DEC struct ctb_arena*	ctb_arena_create(unsigned long long capacity);
CTB_ARENA_DEC struct ctb_arena*	ctb_arena_extend(struct ctb_arena* arena, unsigned long long capacity);
CTB_ARENA_DEC struct ctb_arena*	ctb_arena_reset(struct ctb_arena* arena);
CTB_ARENA_DEC void				ctb_arena_destroy(struct ctb_arena* arena);
CTB_ARENA_DEC struct ctb_arena*	ctb_arena_copy(const struct ctb_arena* source);
CTB_ARENA_DEC char*				ctb_arena_strdup(struct ctb_arena* arena, const char* cstr);

/* Internal helpers are exposed so you can use the header without libc */
CTB_ARENA_DEC void*				ctb_arena_internal_memcpy(void* dst, const void* src,
														   unsigned long long n);
CTB_ARENA_DEC void*				ctb_arena_internal_memset(void* dst, int value,
														   unsigned long long n);
CTB_ARENA_DEC unsigned long long	ctb_arena_internal_strlen(const char* s);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _CTB_ARENA_H */

/* ===================================================================== */
/* ======================= Implementation Section ======================= */
/* ===================================================================== */
#ifdef CTB_ARENA_IMPLEMENTATION

#ifndef CTB_ARENA_DEFAULT_ALIGNMENT
#	define CTB_ARENA_DEFAULT_ALIGNMENT ((unsigned long long)(sizeof(void*)))
#endif

#ifndef CTB_ARENA_DEFAULT_FLAGS
#	define CTB_ARENA_DEFAULT_FLAGS	(0u | CTB_ARENA_FLAG_USE_HEADER | CTB_ARENA_FLAG_MEMSET_ON_SET | \
									CTB_ARENA_FLAG_MEMSET_ON_RESET)
#endif

#if defined(CTB_ARENA_USE_LIBC)
#	include <stdlib.h>
#	include <string.h>
#	ifndef CTB_ARENA_MALLOC
#		define CTB_ARENA_MALLOC(sz)        malloc((sz))
#	endif
#	ifndef CTB_ARENA_FREE
#		define CTB_ARENA_FREE(p)           free((p))
#	endif
#	ifndef CTB_ARENA_MEMCPY
#		define CTB_ARENA_MEMCPY(d,s,n)     memcpy((d),(s),(n))
#	endif
#	ifndef CTB_ARENA_MEMSET
#		define CTB_ARENA_MEMSET(d,v,n)     memset((d),(v),(n))
#	endif
#	ifndef CTB_ARENA_STRLEN
#		define CTB_ARENA_STRLEN(s)         ((unsigned long long)strlen((s)))
#	endif
#else
#	ifndef CTB_ARENA_MALLOC
#		define CTB_ARENA_MALLOC(sz)        0
#	endif
#	ifndef CTB_ARENA_FREE
#		define CTB_ARENA_FREE(p)           ((void)0)
#	endif
#	ifndef CTB_ARENA_MEMCPY
#		define CTB_ARENA_MEMCPY(d,s,n)     ctb_arena_internal_memcpy((d),(s),(n))
#	endif
#	ifndef CTB_ARENA_MEMSET
#		define CTB_ARENA_MEMSET(d,v,n)     ctb_arena_internal_memset((d),(v),(n))
#	endif
#	ifndef CTB_ARENA_STRLEN
#		define CTB_ARENA_STRLEN(s)         ctb_arena_internal_strlen((s))
#	endif
#endif

#if defined(CTB_ARENA_DEBUG) && defined(CTB_ARENA_USE_LIBC)
#	include <stdio.h>
#	define CTB_ARENA_DLOG(...) do { fprintf(stderr, __VA_ARGS__); } while(0)
#else
#	define CTB_ARENA_DLOG(...) ((void)0)
#endif


static int ctb__is_pow2_ull(unsigned long long a) { return a && ((a & (a - 1u)) == 0u); }

static unsigned long long ctb__align_up_ull(unsigned long long x, unsigned long long a)
{
	if (!ctb__is_pow2_ull(a))
	{
		unsigned long long r = x % a;
		return r ? (x + (a - r)) : x;
	}
	unsigned long long m = a - 1u;
	return (x + m) & ~m;
}

CTB_ARENA_DEF
struct ctb_arena* ctb_arena_create(unsigned long long capacity)
{
	struct ctb_arena* arena = (struct ctb_arena*)CTB_ARENA_MALLOC(sizeof(struct ctb_arena));
	if (!arena) return 0;
	arena->flags    = CTB_ARENA_DEFAULT_FLAGS;
	arena->prev     = 0;
	arena->next     = 0;
	arena->offset   = 0;
	arena->capacity = capacity;
	arena->memory   = (unsigned char*)CTB_ARENA_MALLOC(capacity);
	if (!arena->memory)
	{
		CTB_ARENA_FREE(arena);
		return 0;
	}
	if (arena->flags & CTB_ARENA_FLAG_MEMSET_ON_SET) CTB_ARENA_MEMSET(arena->memory, 0, arena->capacity);
	CTB_ARENA_DLOG("[arena] create cap=%llu @%p\n", capacity, (void*)arena);
	return arena;
}

CTB_ARENA_DEF
struct ctb_arena* ctb_arena_extend(struct ctb_arena* arena, unsigned long long capacity)
{
	if (!arena) return 0;
	while (arena->next) arena = arena->next;
	struct ctb_arena* seg = ctb_arena_create(capacity);
	if (!seg) {
		CTB_ARENA_DLOG("[arena] extend failed\n");
		return 0;
	}
	seg->flags = arena->flags;
	arena->next = seg;
	seg->prev = arena;
	CTB_ARENA_DLOG("[arena] extend base=%p new=%p cap=%llu\n", (void*)arena, (void*)seg, capacity);
	return seg;
}


CTB_ARENA_DEF
void* ctb_arena_alloc_aligned(struct ctb_arena* arena, unsigned long long size, unsigned long long alignment)
{
	if (!arena) { return 0; }
	if (alignment == 0) alignment = 1;

	unsigned long long header_size =
		(arena->flags & CTB_ARENA_FLAG_USE_HEADER)
		? (unsigned long long)sizeof(struct ctb_arena_allocation_header)
		: 0ull;

	for (; arena; arena = arena->next)
	{
		unsigned long long base       = (unsigned long long)(arena->memory + arena->offset);
		unsigned long long user_start = ctb__align_up_ull(base + header_size, alignment);
		if (user_start < base)		{ return 0; }
		unsigned long long padding = user_start - base;
		if (padding > ~0ULL - size)	{ return 0; }
		unsigned long long total = padding + size;
		if (total <= arena->capacity - arena->offset)
		{
			unsigned char* user_ptr = arena->memory + arena->offset + padding;

			if (header_size)
			{
				struct ctb_arena_allocation_header h;
				h.size = size;
				CTB_ARENA_MEMCPY((void*)(user_ptr - header_size), &h, header_size);
			}

			arena->offset += total;
			CTB_ARENA_DLOG("[arena] alloc size=%llu align=%llu used=%llu/%llu @%p\n",
				  size, alignment, arena->offset, arena->capacity, (void*)user_ptr);
			return (void*)user_ptr;
		}
		if (!arena->next)
		{
			if (header_size > ~0ULL - size)	{ return 0; }
			unsigned long long grow = header_size + size;
			if (alignment > ~0ULL - grow)	{ return 0; }
			grow += alignment;

			unsigned long long new_cap = (grow > arena->capacity) ? grow : arena->capacity;
			if (!ctb_arena_extend(arena, new_cap)) { return 0; }
		}
	}
	return 0;
}

CTB_ARENA_DEF
void* ctb_arena_alloc(struct ctb_arena* arena, unsigned long long size)
{
	return ctb_arena_alloc_aligned(arena, size, CTB_ARENA_DEFAULT_ALIGNMENT);
}

CTB_ARENA_DEF
void* ctb_arena_realloc(struct ctb_arena* arena, void* old_ptr, unsigned long long new_size)
{
	if (!arena || !old_ptr) return 0;
	if ((arena->flags & CTB_ARENA_FLAG_USE_HEADER) == 0u) {
		return ctb_arena_alloc(arena, new_size);
	}
	struct ctb_arena_allocation_header hdr;
	CTB_ARENA_MEMCPY(&hdr,
					 (unsigned char*)old_ptr - sizeof(struct ctb_arena_allocation_header),
					 sizeof(struct ctb_arena_allocation_header));
	unsigned long long copy_n = (hdr.size < new_size) ? hdr.size : new_size;
	void* p = ctb_arena_alloc(arena, new_size);
	if (!p) return 0;
	CTB_ARENA_MEMCPY(p, old_ptr, copy_n);
	CTB_ARENA_MEMSET((unsigned char*)old_ptr - sizeof(struct ctb_arena_allocation_header),
					 0, hdr.size + sizeof(struct ctb_arena_allocation_header));
	return p;
}

CTB_ARENA_DEF
struct ctb_arena* ctb_arena_reset(struct ctb_arena* arena)
{
	if (!arena) return 0;
	struct ctb_arena* it = arena;
	while (it->prev) it = it->prev;
	for (; it; it = it->next) {
		if (it->flags & CTB_ARENA_FLAG_MEMSET_ON_RESET)
		{
			CTB_ARENA_MEMSET(it->memory, 0, it->capacity);
		}
		it->offset = 0;
	}
	return arena;
}

CTB_ARENA_DEF
void ctb_arena_destroy(struct ctb_arena* arena)
{
	if (!arena) return;
	while (arena->prev) arena = arena->prev;
	struct ctb_arena* it = arena;
	while (it) {
		struct ctb_arena* next = it->next;
		if (it->memory) CTB_ARENA_FREE(it->memory);
		it->memory = 0;
		it->capacity = 0;
		it->offset = 0;
		it->prev = 0;
		it->next = 0;
		CTB_ARENA_DLOG("[arena] destroy @%p\n", (void*)it);
		CTB_ARENA_FREE(it);
		it = next;
	}
}

CTB_ARENA_DEF
struct ctb_arena* ctb_arena_copy(const struct ctb_arena* source)
{
	if (!source) return 0;
	while (source->prev) source = source->prev;
	struct ctb_arena* head = ctb_arena_create(source->capacity);
	if (!head) return 0;
	head->flags = source->flags;
	head->offset = source->offset;
	CTB_ARENA_MEMCPY(head->memory, source->memory, source->offset);
	const struct ctb_arena* s = source->next;
	struct ctb_arena* d = head;
	while (s) {
		struct ctb_arena* seg = ctb_arena_create(s->capacity);
		if (!seg) { ctb_arena_destroy(head); return 0; }
		seg->flags = s->flags;
		seg->offset = s->offset;
		CTB_ARENA_MEMCPY(seg->memory, s->memory, s->offset);
		d->next = seg; seg->prev = d;
		d = seg; s = s->next;
	}
	CTB_ARENA_DLOG("[arena] copy done\n");
	return head;
}

CTB_ARENA_DEF
unsigned long long ctb_arena_internal_strlen(const char* s)
{
	unsigned long long n = 0;
	if (!s) return 0;
	while (s[n]) n++;
	return n;
}

CTB_ARENA_DEF
void* ctb_arena_internal_memcpy(void* dst, const void* src, unsigned long long n)
{
	unsigned long long i;
	unsigned char* d = (unsigned char*)dst;
	const unsigned char* s = (const unsigned char*)src;
	for (i = 0; i < n; ++i) d[i] = s[i];
	return dst;
}

CTB_ARENA_DEF
void* ctb_arena_internal_memset(void* dst, int value, unsigned long long n)
{
	unsigned long long i;
	unsigned char* d = (unsigned char*)dst;
	unsigned char v = (unsigned char)(value & 0xFF);
	for (i = 0; i < n; ++i) d[i] = v;
	return dst;
}

CTB_ARENA_DEF
char* ctb_arena_strdup(struct ctb_arena* arena, const char* cstr)
{
	if (!arena || !cstr) return 0;
	unsigned long long len = CTB_ARENA_STRLEN(cstr);
	char* out = (char*)ctb_arena_alloc(arena, len + 1u);
	if (!out) return 0;
	CTB_ARENA_MEMCPY(out, cstr, len);
	out[len] = '\0';
	return out;
}

#endif /* CTB_ARENA_IMPLEMENTATION */
