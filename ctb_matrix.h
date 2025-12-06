#ifndef _CTB_MATRIX_H
#define _CTB_MATRIX_H


#ifndef CTB_MATRIX_MALLOC
	#include <stdlib.h>
	#define CTB_MATRIX_MALLOC(sz)		malloc(sz)
	#define CTB_MATRIX_FREE(p)			free(p)
	#define CTB_MATRIX_CALLOC(n,sz)		calloc(n,sz)
#endif

#ifndef __has_include
	#define __has_include(x) 0
#endif

#if __has_include("ctb_types.h")
	#include "ctb_types.h"
#else
	typedef		signed char				i8;
	typedef		signed short			i16;
	typedef		signed int				i32;
	typedef		signed long long		i64;
	typedef		unsigned char			u8;
	typedef		unsigned short			u16;
	typedef		unsigned int			u32;
	typedef		unsigned long long		u64;
	typedef		float					f32;
	typedef		double					f64;
#endif


#define CTB_MATRIX_DECL(T)														\
typedef struct ctb_matrix##T													\
{																				\
	u32    rows;																\
	u32    cols;																\
	T      *data;																\
} ctb_matrix##T;																\
																				\
ctb_matrix##T	ctb_matrix##T##_alloc(u32 rows, u32 cols);						\
ctb_matrix##T	ctb_matrix##T##_wrap(u32 rows, u32 cols, T* data);				\
void			ctb_matrix##T##_free(ctb_matrix##T m);							\
T				ctb_matrix##T##_get(ctb_matrix##T m, u32 row, u32 col);			\
void			ctb_matrix##T##_set(ctb_matrix##T m, u32 row, u32 col, T val);	\
ctb_matrix##T	ctb_matrix##T##_mul(ctb_matrix##T a, ctb_matrix##T b);			\
ctb_matrix##T	ctb_matrix##T##_transpose(ctb_matrix##T m);						\
ctb_matrix##T	ctb_matrix##T##_add(ctb_matrix##T a, ctb_matrix##T b);			\
ctb_matrix##T	ctb_matrix##T##_sub(ctb_matrix##T a, ctb_matrix##T b);			\
ctb_matrix##T	ctb_matrix##T##_scale(ctb_matrix##T m, T scalar);				\
ctb_matrix##T	ctb_matrix##T##_identity(u32 n);								\
ctb_matrix##T	ctb_matrix##T##_zero(u32 rows, u32 cols);						\
int				ctb_matrix##T##_eq(ctb_matrix##T a, ctb_matrix##T b);


#define CTB_MATRIX_IMPL(T, ZERO, ONE)											\
ctb_matrix##T ctb_matrix##T##_alloc(u32 rows, u32 cols)							\
{																				\
	ctb_matrix##T m;															\
	m.rows = rows; m.cols = cols;												\
	m.data = (T*)CTB_MATRIX_CALLOC((size_t)(rows * cols), sizeof(T));			\
	return m;																	\
}																				\
																				\
ctb_matrix##T ctb_matrix##T##_wrap(u32 rows, u32 cols, T* data)					\
{																				\
	ctb_matrix##T m;															\
	m.rows = rows;																\
	m.cols = cols;																\
	m.data = data;																\
	return m;																	\
}																				\
																				\
void ctb_matrix##T##_free(ctb_matrix##T m)										\
{																				\
	if (m.data) CTB_MATRIX_FREE(m.data);										\
}																				\
																				\
T ctb_matrix##T##_get(ctb_matrix##T m, u32 row, u32 col)						\
{																				\
	if (!m.data || row >= m.rows || col >= m.cols) return ZERO;					\
	return m.data[col * m.rows + row];											\
}																				\
																				\
void ctb_matrix##T##_set(ctb_matrix##T m, u32 row, u32 col, T val)				\
{																				\
	if (!m.data || row >= m.rows || col >= m.cols) return;						\
	m.data[col * m.rows + row] = val;											\
}																				\
																				\
ctb_matrix##T ctb_matrix##T##_mul(ctb_matrix##T a, ctb_matrix##T b)				\
{																				\
	if (a.cols != b.rows) return ctb_matrix##T##_alloc(0,0);					\
	ctb_matrix##T r = ctb_matrix##T##_alloc(a.rows, b.cols);					\
	if (!r.data) return r;														\
	u32 i, j, k;																\
	for (i = 0; i < b.cols; ++i)												\
	{																			\
		for (j = 0; j < a.rows; ++j)											\
		{																		\
			T sum = ZERO;														\
			for (k = 0; k < a.cols; ++k)										\
			{																	\
				T av = a.data[k * a.rows + j];									\
				T bv = b.data[i * b.rows + k];									\
				sum = sum + (av * bv);											\
			}																	\
			r.data[i * r.rows + j] = sum;										\
		}																		\
	}																			\
	return r;																	\
}																				\
																				\
ctb_matrix##T ctb_matrix##T##_transpose(ctb_matrix##T m)						\
{																				\
	ctb_matrix##T r = ctb_matrix##T##_alloc(m.cols, m.rows);					\
	if (!r.data) return r;														\
	u32 i, j;																	\
	for (i = 0; i < m.cols; ++i)												\
	{																			\
		for (j = 0; j < m.rows; ++j)											\
		{																		\
			r.data[j * r.rows + i] = m.data[i * m.rows + j];					\
		}																		\
	}																			\
	return r;																	\
}																				\
																				\
ctb_matrix##T ctb_matrix##T##_add(ctb_matrix##T a, ctb_matrix##T b)				\
{																				\
	if (a.rows != b.rows || a.cols != b.cols) return ctb_matrix##T##_alloc(0,0);\
	ctb_matrix##T r = ctb_matrix##T##_alloc(a.rows, a.cols);					\
	if (!r.data) return r;														\
	u32 count = a.rows * a.cols;												\
	for (u32 i = 0; i < count; ++i) r.data[i] = a.data[i] + b.data[i];			\
	return r;																	\
}																				\
																				\
ctb_matrix##T ctb_matrix##T##_sub(ctb_matrix##T a, ctb_matrix##T b)				\
{																				\
	if (a.rows != b.rows || a.cols != b.cols) return ctb_matrix##T##_alloc(0,0);\
	ctb_matrix##T r = ctb_matrix##T##_alloc(a.rows, a.cols);					\
	if (!r.data) return r;														\
	u32 count = a.rows * a.cols;												\
	for (u32 i = 0; i < count; ++i) r.data[i] = a.data[i] - b.data[i];			\
	return r;																	\
}																				\
																				\
ctb_matrix##T ctb_matrix##T##_scale(ctb_matrix##T m, T scalar)					\
{																				\
	ctb_matrix##T r = ctb_matrix##T##_alloc(m.rows, m.cols);					\
	if (!r.data) return r;														\
	u32 count = m.rows * m.cols;												\
	for (u32 i = 0; i < count; ++i) r.data[i] = m.data[i] * scalar;				\
	return r;																	\
}																				\
																				\
ctb_matrix##T ctb_matrix##T##_identity(u32 n)									\
{																				\
	ctb_matrix##T r = ctb_matrix##T##_alloc(n, n);								\
	if (!r.data) return r;														\
	for (u32 i = 0; i < n; ++i) r.data[i * n + i] = ONE;						\
	return r;																	\
}																				\
																				\
ctb_matrix##T ctb_matrix##T##_zero(u32 rows, u32 cols)							\
{																				\
	return ctb_matrix##T##_alloc(rows, cols);									\
}																				\
																				\
int ctb_matrix##T##_eq(ctb_matrix##T a, ctb_matrix##T b)						\
{																				\
	if (a.rows != b.rows || a.cols != b.cols) return 0;							\
	if (!a.data || !b.data) return 0;											\
	u32 count = a.rows * a.cols;												\
	for (u32 i=0; i<count; ++i) if (a.data[i] != b.data[i]) return 0;			\
	return 1;																	\
}


CTB_MATRIX_DECL(i8)
CTB_MATRIX_DECL(i16)
CTB_MATRIX_DECL(i32)
CTB_MATRIX_DECL(i64)

CTB_MATRIX_DECL(u8)
CTB_MATRIX_DECL(u16)
CTB_MATRIX_DECL(u32)
CTB_MATRIX_DECL(u64)

CTB_MATRIX_DECL(f32)
CTB_MATRIX_DECL(f64)

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L

#define ctb_matrix_free(m) _Generic((m), \
	ctb_matrixi8:  ctb_matrixi8_free,  ctb_matrixu8:  ctb_matrixu8_free,  \
	ctb_matrixi16: ctb_matrixi16_free, ctb_matrixu16: ctb_matrixu16_free, \
	ctb_matrixi32: ctb_matrixi32_free, ctb_matrixu32: ctb_matrixu32_free, \
	ctb_matrixi64: ctb_matrixi64_free, ctb_matrixu64: ctb_matrixu64_free, \
	ctb_matrixf32: ctb_matrixf32_free, ctb_matrixf64: ctb_matrixf64_free  \
)(m)

#define ctb_matrix_get(m, r, c) _Generic((m), \
	ctb_matrixi8:  ctb_matrixi8_get,  ctb_matrixu8:  ctb_matrixu8_get,  \
	ctb_matrixi16: ctb_matrixi16_get, ctb_matrixu16: ctb_matrixu16_get, \
	ctb_matrixi32: ctb_matrixi32_get, ctb_matrixu32: ctb_matrixu32_get, \
	ctb_matrixi64: ctb_matrixi64_get, ctb_matrixu64: ctb_matrixu64_get, \
	ctb_matrixf32: ctb_matrixf32_get, ctb_matrixf64: ctb_matrixf64_get  \
)(m, r, c)

#define ctb_matrix_set(m, r, c, v) _Generic((m), \
	ctb_matrixi8:  ctb_matrixi8_set,  ctb_matrixu8:  ctb_matrixu8_set,  \
	ctb_matrixi16: ctb_matrixi16_set, ctb_matrixu16: ctb_matrixu16_set, \
	ctb_matrixi32: ctb_matrixi32_set, ctb_matrixu32: ctb_matrixu32_set, \
	ctb_matrixi64: ctb_matrixi64_set, ctb_matrixu64: ctb_matrixu64_set, \
	ctb_matrixf32: ctb_matrixf32_set, ctb_matrixf64: ctb_matrixf64_set  \
)(m, r, c, v)

#define ctb_matrix_mul(a, b) _Generic((a), \
	ctb_matrixi8:  ctb_matrixi8_mul,  ctb_matrixu8:  ctb_matrixu8_mul,  \
	ctb_matrixi16: ctb_matrixi16_mul, ctb_matrixu16: ctb_matrixu16_mul, \
	ctb_matrixi32: ctb_matrixi32_mul, ctb_matrixu32: ctb_matrixu32_mul, \
	ctb_matrixi64: ctb_matrixi64_mul, ctb_matrixu64: ctb_matrixu64_mul, \
	ctb_matrixf32: ctb_matrixf32_mul, ctb_matrixf64: ctb_matrixf64_mul  \
)(a, b)

#define ctb_matrix_transpose(m) _Generic((m), \
	ctb_matrixi8:  ctb_matrixi8_transpose,  ctb_matrixu8:  ctb_matrixu8_transpose,  \
	ctb_matrixi16: ctb_matrixi16_transpose, ctb_matrixu16: ctb_matrixu16_transpose, \
	ctb_matrixi32: ctb_matrixi32_transpose, ctb_matrixu32: ctb_matrixu32_transpose, \
	ctb_matrixi64: ctb_matrixi64_transpose, ctb_matrixu64: ctb_matrixu64_transpose, \
	ctb_matrixf32: ctb_matrixf32_transpose, ctb_matrixf64: ctb_matrixf64_transpose  \
)(m)

#define ctb_matrix_add(a, b) _Generic((a), \
	ctb_matrixi8:  ctb_matrixi8_add,  ctb_matrixu8:  ctb_matrixu8_add,  \
	ctb_matrixi16: ctb_matrixi16_add, ctb_matrixu16: ctb_matrixu16_add, \
	ctb_matrixi32: ctb_matrixi32_add, ctb_matrixu32: ctb_matrixu32_add, \
	ctb_matrixi64: ctb_matrixi64_add, ctb_matrixu64: ctb_matrixu64_add, \
	ctb_matrixf32: ctb_matrixf32_add, ctb_matrixf64: ctb_matrixf64_add  \
)(a, b)

#define ctb_matrix_sub(a, b) _Generic((a), \
	ctb_matrixi8:  ctb_matrixi8_sub,  ctb_matrixu8:  ctb_matrixu8_sub,  \
	ctb_matrixi16: ctb_matrixi16_sub, ctb_matrixu16: ctb_matrixu16_sub, \
	ctb_matrixi32: ctb_matrixi32_sub, ctb_matrixu32: ctb_matrixu32_sub, \
	ctb_matrixi64: ctb_matrixi64_sub, ctb_matrixu64: ctb_matrixu64_sub, \
	ctb_matrixf32: ctb_matrixf32_sub, ctb_matrixf64: ctb_matrixf64_sub  \
)(a, b)

#define ctb_matrix_scale(m, s) _Generic((m), \
	ctb_matrixi8:  ctb_matrixi8_scale,  ctb_matrixu8:  ctb_matrixu8_scale,  \
	ctb_matrixi16: ctb_matrixi16_scale, ctb_matrixu16: ctb_matrixu16_scale, \
	ctb_matrixi32: ctb_matrixi32_scale, ctb_matrixu32: ctb_matrixu32_scale, \
	ctb_matrixi64: ctb_matrixi64_scale, ctb_matrixu64: ctb_matrixu64_scale, \
	ctb_matrixf32: ctb_matrixf32_scale, ctb_matrixf64: ctb_matrixf64_scale  \
)(m, s)

#define ctb_matrix_eq(a, b) _Generic((a), \
	ctb_matrixi8:  ctb_matrixi8_eq,  ctb_matrixu8:  ctb_matrixu8_eq,  \
	ctb_matrixi16: ctb_matrixi16_eq, ctb_matrixu16: ctb_matrixu16_eq, \
	ctb_matrixi32: ctb_matrixi32_eq, ctb_matrixu32: ctb_matrixu32_eq, \
	ctb_matrixi64: ctb_matrixi64_eq, ctb_matrixu64: ctb_matrixu64_eq, \
	ctb_matrixf32: ctb_matrixf32_eq, ctb_matrixf64: ctb_matrixf64_eq  \
)(a, b)

#define ctb_matrix_alloc(T, rows, cols)     ctb_matrix##T##_alloc(rows, cols)
#define ctb_matrix_wrap(T, rows, cols, p)   ctb_matrix##T##_wrap(rows, cols, p)
#define ctb_matrix_identity(T, n)           ctb_matrix##T##_identity(n)
#define ctb_matrix_zero(T, rows, cols)      ctb_matrix##T##_zero(rows, cols)

#endif


#ifdef CTB_MATRIX_NOPREFIX
    typedef ctb_matrixi8  matrixi8;
    typedef ctb_matrixi16 matrixi16;
    typedef ctb_matrixi32 matrixi32;
    typedef ctb_matrixi64 matrixi64;
    typedef ctb_matrixu8  matrixu8;
    typedef ctb_matrixu16 matrixu16;
    typedef ctb_matrixu32 matrixu32;
    typedef ctb_matrixu64 matrixu64;
    typedef ctb_matrixf32 matrixf32;
    typedef ctb_matrixf64 matrixf64;

    #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
        #define matrix_free      ctb_matrix_free
        #define matrix_get       ctb_matrix_get
        #define matrix_set       ctb_matrix_set
        #define matrix_mul       ctb_matrix_mul
        #define matrix_transpose ctb_matrix_transpose
        #define matrix_add       ctb_matrix_add
        #define matrix_sub       ctb_matrix_sub
        #define matrix_scale     ctb_matrix_scale
        #define matrix_eq        ctb_matrix_eq

        #define matrix_alloc     ctb_matrix_alloc
        #define matrix_wrap      ctb_matrix_wrap
        #define matrix_identity  ctb_matrix_identity
        #define matrix_zero      ctb_matrix_zero
    #endif
#endif
#endif /* _CTB_MATRIX_H */

#ifdef CTB_MATRIX_IMPLEMENTATION

	CTB_MATRIX_IMPL(i8,  0, 1)
	CTB_MATRIX_IMPL(i16, 0, 1)
	CTB_MATRIX_IMPL(i32, 0, 1)
	CTB_MATRIX_IMPL(i64, 0, 1)

	CTB_MATRIX_IMPL(u8,  0, 1)
	CTB_MATRIX_IMPL(u16, 0, 1)
	CTB_MATRIX_IMPL(u32, 0, 1)
	CTB_MATRIX_IMPL(u64, 0, 1)

	CTB_MATRIX_IMPL(f32, 0.0f, 1.0f)
	CTB_MATRIX_IMPL(f64, 0.0,  1.0)

#endif /* CTB_MATRIX_IMPLEMENTATION */

