#ifndef _CTB_DA_H
#define _CTB_DA_H

#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CTB_DA_DECL(T, Name)	\
typedef struct Name				\
{								\
	T*		data;				\
	size_t	count;				\
	size_t	capacity;			\
} Name


#define ctb_da_init(a, cap) \
_ctb_da_init_internal((void**)&((a)->data), &((a)->count), &((a)->capacity), (cap), sizeof(*((a)->data)))

#define ctb_da_free(a) \
_ctb_da_free_internal((void**)&((a)->data), &((a)->count), &((a)->capacity))

#define ctb_da_append(a, val) \
(ctb_da_full(a) ? _ctb_da_grow_internal((void**)&((a)->data), &((a)->count), &((a)->capacity), sizeof(*((a)->data))) : 0, \
	(a)->data[(a)->count++] = (val))

#define ctb_da_full(a)		((a)->count >= (a)->capacity)
#define ctb_da_clear(a)		((a)->count = 0)
#define ctb_da_last(a)		((a)->data[(a)->count - 1])
#define ctb_da_pop(a)		((a)->count > 0 ? (a)->count-- : 0)


#if defined(CTB_DA_STATIC)
#  define CTB_DA_DEC static
#  define CTB_DA_DEF static
#elif defined(__cplusplus)
#  define CTB_DA_DEC extern "C"
#  define CTB_DA_DEF extern "C"
#else
#  define CTB_DA_DEC extern
#  define CTB_DA_DEF
#endif

CTB_DA_DEC int  _ctb_da_grow_internal(void** data, size_t* count, size_t* cap, size_t elem_size);
CTB_DA_DEC void _ctb_da_init_internal(void** data, size_t* count, size_t* cap, size_t initial_cap, size_t elem_size);
CTB_DA_DEC void _ctb_da_free_internal(void** data, size_t* count, size_t* cap);

#ifdef CTB_DA_NOPREFIX
	#define da_decl(T, N)	CTB_DA_DECL(T, N)
	#define da_init			ctb_da_init
	#define da_free			ctb_da_free
	#define da_append		ctb_da_append
	#define da_clear		ctb_da_clear
	#define da_last			ctb_da_last
	#define da_pop			ctb_da_pop
#endif

#ifdef __cplusplus
}
#endif
#endif /* _CTB_DA_H */

#ifdef CTB_DA_IMPLEMENTATION

#include <stdlib.h>

#ifndef CTB_DA_MALLOC
#define CTB_DA_MALLOC(sz)		malloc(sz)
#define CTB_DA_REALLOC(p,sz)	realloc(p,sz)
#define CTB_DA_FREE(p)			free(p)
#endif

CTB_DA_DEF
void _ctb_da_init_internal(void** data, size_t* count, size_t* cap, size_t initial_cap, size_t elem_size)
{
	*count = 0;
	*cap   = initial_cap;
	if (initial_cap > 0)
	{
		*data = CTB_DA_MALLOC(initial_cap * elem_size);
	}
	else
	{
		*data = NULL;
	}
}

CTB_DA_DEF
void _ctb_da_free_internal(void** data, size_t* count, size_t* cap)
{
	if (*data) CTB_DA_FREE(*data);
	*data	= NULL;
	*count	= 0;
	*cap	= 0;
}

CTB_DA_DEF
int _ctb_da_grow_internal(void** data, size_t* count, size_t* cap, size_t elem_size)
{
	size_t new_cap	= (*cap == 0) ? 4 : (*cap * 2);
	void* new_data	= CTB_DA_REALLOC(*data, new_cap * elem_size);
	if (!new_data) return 0;

	*data	= new_data;
	*cap	= new_cap;
	return 1;
}

#endif /* CTB_DA_IMPLEMENTATION */
