#ifndef _CTB_TYPES_H
#define _CTB_TYPES_H

#ifndef __has_include
	#define __has_include(x) 0
#endif

#if __has_include(<stdint.h>)
	#include <stdint.h>
	typedef	int8_t		i8;
	typedef	int16_t		i16;
	typedef	int32_t		i32;
	typedef	int64_t		i64;

	typedef	uint8_t		u8;
	typedef	uint16_t	u16;
	typedef	uint32_t	u32;
	typedef	uint64_t	u64;
#else
	typedef	signed char			i8;
	typedef	signed short		i16;
	typedef	signed int			i32;
	typedef	signed long long	i64;

	typedef	unsigned char		u8;
	typedef	unsigned short		u16;
	typedef	unsigned int		u32;
	typedef	unsigned long long	u64;
#endif

typedef	float				f32;
typedef	double				f64;

#endif
