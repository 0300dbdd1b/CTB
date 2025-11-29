#ifndef _CTB_MATRIX_H
#define _CTB_MATRIX_H

#ifdef __cplusplus
	extern "C" {
#endif


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

// NOTE: Can we do this better?
#ifdef CTB_MATRIX_NOPREFIX
    typedef ctb_matrixf32 matrixf32;
    #define matrixf32_alloc       ctb_matrixf32_alloc
    #define matrixf32_wrap        ctb_matrixf32_wrap
    #define matrixf32_free        ctb_matrixf32_free
    #define matrixf32_get         ctb_matrixf32_get
    #define matrixf32_set         ctb_matrixf32_set
    #define matrixf32_mul         ctb_matrixf32_mul
    #define matrixf32_transpose   ctb_matrixf32_transpose
    #define matrixf32_add         ctb_matrixf32_add
    #define matrixf32_sub         ctb_matrixf32_sub
    #define matrixf32_scale       ctb_matrixf32_scale
    #define matrixf32_identity    ctb_matrixf32_identity
    #define matrixf32_zero        ctb_matrixf32_zero
    #define matrixf32_eq          ctb_matrixf32_eq

    typedef ctb_matrixf64 matrixf64;
    #define matrixf64_alloc       ctb_matrixf64_alloc
    #define matrixf64_wrap        ctb_matrixf64_wrap
    #define matrixf64_free        ctb_matrixf64_free
    #define matrixf64_get         ctb_matrixf64_get
    #define matrixf64_set         ctb_matrixf64_set
    #define matrixf64_mul         ctb_matrixf64_mul
    #define matrixf64_transpose   ctb_matrixf64_transpose
    #define matrixf64_add         ctb_matrixf64_add
    #define matrixf64_sub         ctb_matrixf64_sub
    #define matrixf64_scale       ctb_matrixf64_scale
    #define matrixf64_identity    ctb_matrixf64_identity
    #define matrixf64_zero        ctb_matrixf64_zero
    #define matrixf64_eq          ctb_matrixf64_eq


    typedef ctb_matrixi8 matrixi8;
    #define matrixi8_alloc       ctb_matrixi8_alloc
    #define matrixi8_wrap        ctb_matrixi8_wrap
    #define matrixi8_free        ctb_matrixi8_free
    #define matrixi8_get         ctb_matrixi8_get
    #define matrixi8_set         ctb_matrixi8_set
    #define matrixi8_mul         ctb_matrixi8_mul
    #define matrixi8_transpose   ctb_matrixi8_transpose
    #define matrixi8_add         ctb_matrixi8_add
    #define matrixi8_sub         ctb_matrixi8_sub
    #define matrixi8_scale       ctb_matrixi8_scale
    #define matrixi8_identity    ctb_matrixi8_identity
    #define matrixi8_zero        ctb_matrixi8_zero
    #define matrixi8_eq          ctb_matrixi8_eq

    typedef ctb_matrixi16 matrixi16;
    #define matrixi16_alloc       ctb_matrixi16_alloc
    #define matrixi16_wrap        ctb_matrixi16_wrap
    #define matrixi16_free        ctb_matrixi16_free
    #define matrixi16_get         ctb_matrixi16_get
    #define matrixi16_set         ctb_matrixi16_set
    #define matrixi16_mul         ctb_matrixi16_mul
    #define matrixi16_transpose   ctb_matrixi16_transpose
    #define matrixi16_add         ctb_matrixi16_add
    #define matrixi16_sub         ctb_matrixi16_sub
    #define matrixi16_scale       ctb_matrixi16_scale
    #define matrixi16_identity    ctb_matrixi16_identity
    #define matrixi16_zero        ctb_matrixi16_zero
    #define matrixi16_eq          ctb_matrixi16_eq

    typedef ctb_matrixi32 matrixi32;
    #define matrixi32_alloc       ctb_matrixi32_alloc
    #define matrixi32_wrap        ctb_matrixi32_wrap
    #define matrixi32_free        ctb_matrixi32_free
    #define matrixi32_get         ctb_matrixi32_get
    #define matrixi32_set         ctb_matrixi32_set
    #define matrixi32_mul         ctb_matrixi32_mul
    #define matrixi32_transpose   ctb_matrixi32_transpose
    #define matrixi32_add         ctb_matrixi32_add
    #define matrixi32_sub         ctb_matrixi32_sub
    #define matrixi32_scale       ctb_matrixi32_scale
    #define matrixi32_identity    ctb_matrixi32_identity
    #define matrixi32_zero        ctb_matrixi32_zero
    #define matrixi32_eq          ctb_matrixi32_eq


    typedef ctb_matrixi64 matrixi64;
    #define matrixi64_alloc       ctb_matrixi64_alloc
    #define matrixi64_wrap        ctb_matrixi64_wrap
    #define matrixi64_free        ctb_matrixi64_free
    #define matrixi64_get         ctb_matrixi64_get
    #define matrixi64_set         ctb_matrixi64_set
    #define matrixi64_mul         ctb_matrixi64_mul
    #define matrixi64_transpose   ctb_matrixi64_transpose
    #define matrixi64_add         ctb_matrixi64_add
    #define matrixi64_sub         ctb_matrixi64_sub
    #define matrixi64_scale       ctb_matrixi64_scale
    #define matrixi64_identity    ctb_matrixi64_identity
    #define matrixi64_zero        ctb_matrixi64_zero
    #define matrixi64_eq          ctb_matrixi64_eq

    typedef ctb_matrixu8 matrixu8;
    #define matrixu8_alloc       ctb_matrixu8_alloc
    #define matrixu8_wrap        ctb_matrixu8_wrap
    #define matrixu8_free        ctb_matrixu8_free
    #define matrixu8_get         ctb_matrixu8_get
    #define matrixu8_set         ctb_matrixu8_set
    #define matrixu8_mul         ctb_matrixu8_mul
    #define matrixu8_transpose   ctb_matrixu8_transpose
    #define matrixu8_add         ctb_matrixu8_add
    #define matrixu8_sub         ctb_matrixu8_sub
    #define matrixu8_scale       ctb_matrixu8_scale
    #define matrixu8_identity    ctb_matrixu8_identity
    #define matrixu8_zero        ctb_matrixu8_zero
    #define matrixu8_eq          ctb_matrixu8_eq

    typedef ctb_matrixu16 matrixu16;
    #define matrixu16_alloc       ctb_matrixu16_alloc
    #define matrixu16_wrap        ctb_matrixu16_wrap
    #define matrixu16_free        ctb_matrixu16_free
    #define matrixu16_get         ctb_matrixu16_get
    #define matrixu16_set         ctb_matrixu16_set
    #define matrixu16_mul         ctb_matrixu16_mul
    #define matrixu16_transpose   ctb_matrixu16_transpose
    #define matrixu16_add         ctb_matrixu16_add
    #define matrixu16_sub         ctb_matrixu16_sub
    #define matrixu16_scale       ctb_matrixu16_scale
    #define matrixu16_identity    ctb_matrixu16_identity
    #define matrixu16_zero        ctb_matrixu16_zero
    #define matrixu16_eq          ctb_matrixu16_eq

    typedef ctb_matrixu32 matrixu32;
    #define matrixu32_alloc       ctb_matrixu32_alloc
    #define matrixu32_wrap        ctb_matrixu32_wrap
    #define matrixu32_free        ctb_matrixu32_free
    #define matrixu32_get         ctb_matrixu32_get
    #define matrixu32_set         ctb_matrixu32_set
    #define matrixu32_mul         ctb_matrixu32_mul
    #define matrixu32_transpose   ctb_matrixu32_transpose
    #define matrixu32_add         ctb_matrixu32_add
    #define matrixu32_sub         ctb_matrixu32_sub
    #define matrixu32_scale       ctb_matrixu32_scale
    #define matrixu32_identity    ctb_matrixu32_identity
    #define matrixu32_zero        ctb_matrixu32_zero
    #define matrixu32_eq          ctb_matrixu32_eq

    typedef ctb_matrixu64 matrixu64;
    #define matrixu64_alloc       ctb_matrixu64_alloc
    #define matrixu64_wrap        ctb_matrixu64_wrap
    #define matrixu64_free        ctb_matrixu64_free
    #define matrixu64_get         ctb_matrixu64_get
    #define matrixu64_set         ctb_matrixu64_set
    #define matrixu64_mul         ctb_matrixu64_mul
    #define matrixu64_transpose   ctb_matrixu64_transpose
    #define matrixu64_add         ctb_matrixu64_add
    #define matrixu64_sub         ctb_matrixu64_sub
    #define matrixu64_scale       ctb_matrixu64_scale
    #define matrixu64_identity    ctb_matrixu64_identity
    #define matrixu64_zero        ctb_matrixu64_zero
    #define matrixu64_eq          ctb_matrixu64_eq
#endif

#ifdef __cplusplus
}
#endif

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

#endif /* _CTB_MATRIX_H */
