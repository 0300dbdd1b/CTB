#ifndef _CTB_MACROS_H
#define _CTB_MACROS_H

#ifdef __cplusplus
	extern "C" {
#endif

#define CTB_KiB	(1024ULL)
#define CTB_MiB	(1024ULL * CTB_KiB)
#define CTB_GiB	(1024ULL * CTB_MiB)
#define CTB_TiB	(1024ULL * CTB_GiB)
#define CTB_PiB	(1024ULL * CTB_TiB)

#define CTB_KB	(1000ULL)
#define CTB_MB	(1000ULL * CTB_KB)
#define CTB_GB	(1000ULL * CTB_MB)
#define CTB_TB	(1000ULL * CTB_GB)
#define CTB_PB	(1000ULL * CTB_TB)


#define CTB_SET_BIT(v, bit)		((v) |= (1ULL << (bit)))
#define CTB_CLEAR_BIT(v, bit)	((v) &= ~(1ULL << (bit)))
#define CTB_TOGGLE_BIT(v, bit)	((v) ^= (1ULL << (bit)))
#define CTB_IS_BIT_SET(v, bit)	(!!(v) & (1ULL << (bit)))

#define CTB_COUNTOF(x)	(sizeof(x)/sizeof((x)[0]))
#define CTB_UNUSED(x)	((void)(x))

#ifdef CTB_MACROS_NOPREFIX
	#define KiB		CTB_KiB
	#define MiB		CTB_MiB
	#define GiB		CTB_GiB
	#define TiB		CTB_TiB
	#define PiB		CTB_PiB

	#define KB		CTB_KB
	#define MB		CTB_MB
	#define GB		CTB_GB
	#define TB		CTB_TB
	#define PB		CTB_PB

	#define SET_BIT		CTB_SET_BIT
	#define CLEAR_BIT	CTB_CLEAR_BIT
	#define TOGGLE_BIT	CTB_TOGGLE_BIT
	#define IS_BIT_SET	CTB_IS_BIT_SET

	#define UNUSED		CTB_UNUSED
	#define COUNTOF		CTB_COUNTOF
#endif


#ifdef __cplusplus
} /* extern "C" */
#endif




#endif // !_CTB_MACROS_H
