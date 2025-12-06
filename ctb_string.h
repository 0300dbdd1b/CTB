#ifndef _CTB_STRING_H
#define _CTB_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CTB_STRING_STATIC)
#  define CTB_STRING_DEC static
#  define CTB_STRING_DEF static
#elif defined(__cplusplus)
#  define CTB_STRING_DEC extern "C"
#  define CTB_STRING_DEF extern "C"
#else
#  define CTB_STRING_DEC extern
#  define CTB_STRING_DEF
#endif

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _ctb_string_header
{
	size_t len;
	size_t cap;
} _ctb_string_header;

typedef char * ctb_string;

#define CTB_STR_HEADER(s)	(((_ctb_string_header *)(void *)(s)) - 1)
#define CTB_STR_LEN(s)		((s) ? CTB_STR_HEADER(s)->len : 0)
#define CTB_STR_CAP(s)		((s) ? CTB_STR_HEADER(s)->cap : 0)

CTB_STRING_DEC ctb_string	ctb_string_create(const char *s);
CTB_STRING_DEC ctb_string	ctb_string_create_len(const char *s, size_t len);
CTB_STRING_DEC void			ctb_string_free(ctb_string s);

CTB_STRING_DEC ctb_string	ctb_string_reserve(ctb_string s, size_t new_cap);

CTB_STRING_DEC ctb_string	ctb_string_append(ctb_string s, const char *suffix);
CTB_STRING_DEC ctb_string	ctb_string_append_len(ctb_string s, const char *buf, size_t len);
CTB_STRING_DEC ctb_string	ctb_string_push_char(ctb_string s, char c);
CTB_STRING_DEC void			ctb_string_clear(ctb_string s);

#ifdef CTB_STRING_NOPREFIX
	#define string_create		ctb_string_create
	#define string_create_len	ctb_string_create_len
	#define string_free			ctb_string_free
	#define string_reserve		ctb_string_reserve
	#define string_append		ctb_string_append
	#define string_append_len	ctb_string_append_len
	#define string_push_char	ctb_string_push_char
	#define string_clear		ctb_string_clear
#endif

#ifdef __cplusplus
}
#endif


#ifdef CTB_STRING_IMPLEMENTATION

#ifndef CTB_STRING_EXTRA_SPACE
#  define CTB_STRING_EXTRA_SPACE 16
#endif

CTB_STRING_DEF
static ctb_string _ctb_string_alloc(size_t initial_len)
{
	size_t cap = initial_len + CTB_STRING_EXTRA_SPACE;
	if (cap == 0) cap = CTB_STRING_EXTRA_SPACE;

	_ctb_string_header *header = (_ctb_string_header *)calloc(1, sizeof(_ctb_string_header) + cap + 1);
	if (!header) return NULL;

	header->len = initial_len;
	header->cap = cap;
	ctb_string s = (char *)(header + 1);
	s[initial_len] = '\0';
	return s;
}

CTB_STRING_DEF
ctb_string ctb_string_create_len(const char *s, size_t len)
{
	ctb_string str = _ctb_string_alloc(len);
	if (!str) return NULL;

	if (s && len)
	{
		memcpy(str, s, len);
	}
	return str;
}

CTB_STRING_DEF
ctb_string ctb_string_create(const char *s)
{
	size_t len = s ? strlen(s) : 0;
	return ctb_string_create_len(s, len);
}

CTB_STRING_DEF
void ctb_string_free(ctb_string s)
{
	if (!s) return;
	_ctb_string_header *header = CTB_STR_HEADER(s);
	free(header);
}

CTB_STRING_DEF
ctb_string ctb_string_reserve(ctb_string s, size_t new_cap)
{
	if (!s)
	{
		ctb_string ns = _ctb_string_alloc(0);
		if (!ns) return NULL;
		_ctb_string_header *h = CTB_STR_HEADER(ns);
		if (h->cap < new_cap)
		{
			size_t cap = new_cap;
			_ctb_string_header *h2 = (_ctb_string_header *)realloc(h, sizeof(_ctb_string_header) + cap + 1);
			if (!h2)
			{
				free(h);
				return NULL;
			}
			h2->cap = cap;
			ns = (char *)(h2 + 1);
		}
		return ns;
	}

	_ctb_string_header *header = CTB_STR_HEADER(s);
	if (new_cap <= header->cap)
		return s;

	size_t cap = new_cap;
	_ctb_string_header *new_header = (_ctb_string_header *)realloc(header, sizeof(_ctb_string_header) + cap + 1);
	if (!new_header) return NULL;

	new_header->cap = cap;
	return (char *)(new_header + 1);
}

CTB_STRING_DEF
ctb_string ctb_string_append_len(ctb_string s, const char *buf, size_t len)
{
	if (!buf || len == 0)	return s;
	if (!s)					return ctb_string_create_len(buf, len);
	
	_ctb_string_header *header = CTB_STR_HEADER(s);
	size_t old_len = header->len;
	size_t new_len = old_len + len;

	if (new_len + 1 > header->cap)
	{
		size_t new_cap = header->cap * 2;
		if (new_cap < new_len + 1)
		{
			new_cap = new_len + 1;
		}
		s = ctb_string_reserve(s, new_cap);
		if (!s)	return NULL;
		header = CTB_STR_HEADER(s);
	}

	memcpy(s + old_len, buf, len);
	s[new_len] = '\0';
	header->len = new_len;

	return s;
}

CTB_STRING_DEF
ctb_string ctb_string_append(ctb_string s, const char *suffix)
{
	if (!suffix) return s;
	return ctb_string_append_len(s, suffix, strlen(suffix));
}

CTB_STRING_DEF
ctb_string ctb_string_push_char(ctb_string s, char ch)
{
	return ctb_string_append_len(s, &ch, 1);
}

CTB_STRING_DEF
void ctb_string_clear(ctb_string s)
{
	if (!s) return;
	_ctb_string_header *header = CTB_STR_HEADER(s);
	header->len = 0;
	s[0] = '\0';
}

#endif /* CTB_STRING_IMPLEMENTATION */

#endif /* _CTB_STRING_H */
