#ifndef _CTB_PLATFORM_H
#define _CTB_PLATFORM_H

#if defined(_WIN32) || defined(_WIN64)
	#define CTB_OS_WINDOWS 1
#elif defined(__APPLE__) || defined(__MACH__)
	#define CTB_OS_MACOS   1
	#define CTB_OS_UNIX    1
#elif defined(__ANDROID__)
	#define CTB_OS_ANDROID 1
	#define CTB_OS_LINUX   1
	#define CTB_OS_UNIX    1
#elif defined(__linux__)
	#define CTB_OS_LINUX   1
	#define CTB_OS_UNIX    1
#elif defined(__unix__) || defined(__unix)
	#define CTB_OS_UNIX    1
#else
	#define CTB_OS_UNKNOWN 1
#endif

#ifndef CTB_OS_WINDOWS
	#define CTB_OS_WINDOWS 0
#endif
#ifndef CTB_OS_MACOS
	#define CTB_OS_MACOS 0
#endif
#ifndef CTB_OS_LINUX
	#define CTB_OS_LINUX 0
#endif
#ifndef CTB_OS_UNIX
	#define CTB_OS_UNIX 0
#endif
#ifndef CTB_OS_ANDROID
	#define CTB_OS_ANDROID 0
#endif

#if defined(__clang__)
	#define CTB_COMPILER_CLANG 1
#elif defined(__GNUC__)
	#define CTB_COMPILER_GCC   1
#elif defined(_MSC_VER)
	#define CTB_COMPILER_MSVC  1
#else
	#define CTB_COMPILER_UNKNOWN 1
#endif

#ifndef CTB_COMPILER_CLANG
	#define CTB_COMPILER_CLANG 0
#endif
#ifndef CTB_COMPILER_GCC
	#define CTB_COMPILER_GCC 0
#endif
#ifndef CTB_COMPILER_MSVC
	#define CTB_COMPILER_MSVC 0
#endif

#if defined(__x86_64__) || defined(_M_X64)
	#define CTB_ARCH_X64 1
#elif defined(__i386__) || defined(_M_IX86)
	#define CTB_ARCH_X86 1
#elif defined(__aarch64__) || defined(_M_ARM64)
	#define CTB_ARCH_ARM64 1
#elif defined(__arm__) || defined(_M_ARM)
	#define CTB_ARCH_ARM32 1
#else
	#define CTB_ARCH_UNKNOWN 1
#endif

#if defined(__STDC_VERSION__)
	#define CTB_C_VERSION __STDC_VERSION__
#elif defined(__cplusplus)
	#define CTB_C_VERSION 0
#elif defined(_MSC_VER)
	#define CTB_C_VERSION 199001L
#else
	#define CTB_C_VERSION 199001L
#endif

#define CTB_C89 199001L
#define CTB_C99 199901L
#define CTB_C11 201112L
#define CTB_C17 201710L
#define CTB_C23 202311L

#if defined(__has_include)
	#define CTB_HAS_INCLUDE(x)	__has_include(x)
#else
	#define CTB_HAS_INCLUDE(x)	0
#endif

#if defined(__has_attribute)
	#define CTB_HAS_ATTRIBUTE(x)	__has_attribute(x)
#else
	#define CTB_HAS_ATTRIBUTE(x)	0
#endif

#if defined(__has_builtin)
	#define CTB_HAS_BUILTIN(x)	__has_builtin(x)
#else
	#define CTB_HAS_BUILTIN(x)	0
#endif

#if defined(__has_feature)
	#define CTB_HAS_FEATURE(x)	__has_feature(x)
#else
	#define CTB_HAS_FEATURE(x)	0
#endif

#if CTB_COMPILER_MSVC
	#define CTB_INLINE			__inline
	#define CTB_FORCE_INLINE	__forceinline
	#define CTB_NOINLINE		__declspec(noinline)
#elif CTB_COMPILER_GCC || CTB_COMPILER_CLANG
	#define CTB_INLINE			inline
	#define CTB_FORCE_INLINE	inline __attribute__((always_inline))
	#define CTB_NOINLINE		__attribute__((noinline))
#else
	#define CTB_INLINE			inline
	#define CTB_FORCE_INLINE	inline
	#define CTB_NOINLINE
#endif

#if CTB_COMPILER_GCC || CTB_COMPILER_CLANG
	#define CTB_LIKELY(x)		__builtin_expect(!!(x), 1)
	#define CTB_UNLIKELY(x)		__builtin_expect(!!(x), 0)
#else
	#define CTB_LIKELY(x)		(x)
	#define CTB_UNLIKELY(x)		(x)
#endif

#if CTB_COMPILER_MSVC
	#define CTB_UNREACHABLE()	__assume(0)
#elif CTB_COMPILER_GCC || CTB_COMPILER_CLANG
	#define CTB_UNREACHABLE()	__builtin_unreachable()
#else
	#define CTB_UNREACHABLE()
#endif

#if CTB_COMPILER_MSVC
	#define CTB_DEBUG_TRAP()	__debugbreak()
#elif CTB_COMPILER_CLANG
	#define CTB_DEBUG_TRAP()	__builtin_debugtrap()
#elif CTB_COMPILER_GCC
	#define CTB_DEBUG_TRAP()	__builtin_trap()
#else
	#define CTB_DEBUG_TRAP()	(*(volatile int *)0 = 0)
#endif

#if CTB_COMPILER_MSVC
	#define CTB_DEPRECATED			__declspec(deprecated)
	#define CTB_DEPRECATED_MSG(m)	__declspec(deprecated(m))
#elif CTB_COMPILER_GCC || CTB_COMPILER_CLANG
	#define CTB_DEPRECATED			__attribute__((deprecated))
	#define CTB_DEPRECATED_MSG(m)	__attribute__((deprecated(m)))
#else
	#define CTB_DEPRECATED
	#define CTB_DEPRECATED_MSG(m)
#endif

#if CTB_COMPILER_MSVC
	#define CTB_NORETURN		__declspec(noreturn)
#elif CTB_COMPILER_GCC || CTB_COMPILER_CLANG
	#define CTB_NORETURN		__attribute__((noreturn))
#elif CTB_C_VERSION >= CTB_C11
	#define CTB_NORETURN		_Noreturn
#else
	#define CTB_NORETURN
#endif

#if CTB_COMPILER_MSVC
	#define CTB_THREAD_LOCAL	__declspec(thread)
#elif CTB_C_VERSION >= CTB_C11
	#define CTB_THREAD_LOCAL	_Thread_local
#elif CTB_COMPILER_GCC || CTB_COMPILER_CLANG
	#define CTB_THREAD_LOCAL	__thread
#else
	#define CTB_THREAD_LOCAL
#endif

#if CTB_C_VERSION >= CTB_C11
	#define CTB_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
	#define CTB_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#else
	#define _CTB_SA_CAT(a, b) a##b
	#define _CTB_SA_XCAT(a, b) _CTB_SA_CAT(a, b)
	#define CTB_STATIC_ASSERT(cond, msg) \
	typedef char _CTB_SA_XCAT(static_assertion_, __LINE__)[(cond)?1:-1]
#endif

#ifdef CTB_PLATFORM_NOPREFIX
	#define OS_WINDOWS		CTB_OS_WINDOWS
	#define OS_MACOS		CTB_OS_MACOS
	#define OS_LINUX		CTB_OS_LINUX
	#define OS_UNIX			CTB_OS_UNIX
	#define OS_ANDROID		CTB_OS_ANDROID

	#define ARCH_X64		CTB_ARCH_X64
	#define ARCH_X86		CTB_ARCH_X86
	#define ARCH_ARM64		CTB_ARCH_ARM64
	#define ARCH_ARM32		CTB_ARCH_ARM32

	#define HAS_INCLUDE		CTB_HAS_INCLUDE
	#define HAS_ATTRIBUTE	CTB_HAS_ATTRIBUTE
	#define HAS_BUILTIN		CTB_HAS_BUILTIN
	#define HAS_FEATURE		CTB_HAS_FEATURE

	#define INLINE			CTB_INLINE
	#define FORCE_INLINE	CTB_FORCE_INLINE
	#define NOINLINE		CTB_NOINLINE
	#define UNREACHABLE		CTB_UNREACHABLE
	#define DEBUG_TRAP		CTB_DEBUG_TRAP
	#define DEPRECATED		CTB_DEPRECATED
	#define NORETURN		CTB_NORETURN
	#define THREAD_LOCAL	CTB_THREAD_LOCAL
	#define STATIC_ASSERT	CTB_STATIC_ASSERT
	#define LIKELY			CTB_LIKELY
	#define UNLIKELY		CTB_UNLIKELY
#endif

#endif /* _CTB_PLATFORM_H */
