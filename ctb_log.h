#ifndef _CTB_LOG_H
#define _CTB_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#if defined(_WIN32)
	#define _CTB_COLOR_RESET    ""
	#define _CTB_COLOR_GRAY     ""
	#define _CTB_COLOR_ORANGE   ""
	#define _CTB_COLOR_CYAN     ""
	#define _CTB_COLOR_YELLOW   ""
	#define _CTB_COLOR_RED      ""
	#define _CTB_COLOR_MAGENTA  ""
#else
	#define _CTB_COLOR_RESET    "\x1b[0m"
	#define _CTB_COLOR_GRAY     "\x1b[90m"
	#define _CTB_COLOR_ORANGE   "\x1b[38;5;208m"
	#define _CTB_COLOR_CYAN     "\x1b[36m"
	#define _CTB_COLOR_YELLOW   "\x1b[33m"
	#define _CTB_COLOR_RED      "\x1b[31m"
	#define _CTB_COLOR_MAGENTA  "\x1b[35m"
#endif

#define CTB_LOG_LEVELS(X)								\
	X(NONE,		"NONE",		0,		_CTB_COLOR_RESET)	\
	X(TRACE,	"TRACE",	1,		_CTB_COLOR_GRAY)	\
	X(DEBUG,	"DEBUG",	2,		_CTB_COLOR_ORANGE)	\
	X(INFO,		"INFO",		4,		_CTB_COLOR_CYAN)	\
	X(WARN,		"WARN",		8,		_CTB_COLOR_YELLOW)	\
	X(ERROR,	"ERROR",	16,		_CTB_COLOR_RED)		\
	X(FATAL,	"FATAL",	32,		_CTB_COLOR_MAGENTA)	\
	X(CMD,		"CMD",		64,		_CTB_COLOR_CYAN)

enum ctb_log_level
{
	#define X(sym, name, val, color) CTB_LOG_##sym = val,
		CTB_LOG_LEVELS(X)
	#undef X
};

#define CTB_LOG_ALL (CTB_LOG_NONE | CTB_LOG_TRACE | CTB_LOG_DEBUG | CTB_LOG_INFO | CTB_LOG_WARN | CTB_LOG_ERROR | CTB_LOG_FATAL | CTB_LOG_CMD)

#if defined(CTB_LOG_STATIC)
#  define CTB_LOG_DEC static
#  define CTB_LOG_DEF static
#elif defined(__cplusplus)
#  define CTB_LOG_DEC extern "C"
#  define CTB_LOG_DEF extern "C"
#else
#  define CTB_LOG_DEC extern
#  define CTB_LOG_DEF
#endif

CTB_LOG_DEC void ctb_log_set_mask(unsigned int mask);
CTB_LOG_DEC void ctb_log_enable(unsigned int flags);
CTB_LOG_DEC void ctb_log_disable(unsigned int flags);
CTB_LOG_DEC void ctb_log_printf(int level, const char* fmt, ...);

#define ctb_log(lvl, ...) ctb_log_printf((lvl), __VA_ARGS__)

#ifdef CTB_LOG_NOPREFIX
	#define LOG_NONE		CTB_LOG_NONE
	#define LOG_TRACE		CTB_LOG_TRACE
	#define LOG_DEBUG		CTB_LOG_DEBUG
	#define LOG_INFO		CTB_LOG_INFO
	#define LOG_WARN		CTB_LOG_WARN
	#define LOG_ERROR		CTB_LOG_ERROR
	#define LOG_FATAL		CTB_LOG_FATAL
	#define LOG_CMD			CTB_LOG_CMD
	#define LOG_ALL			CTB_LOG_ALL
	#define log_set_mask	ctb_log_set_mask
	#define log_enable		ctb_log_enable
	#define log_disable		ctb_log_disable
	#define log_printf		ctb_log_printf
	#define log_msg			ctb_log
#endif

#ifdef __cplusplus
}
#endif
#endif /* _CTB_LOG_H */

#ifdef CTB_LOG_IMPLEMENTATION

#include <stdio.h>

static unsigned int _ctb_log_mask = CTB_LOG_ALL;

static const char* _ctb_log_get_name(int lvl)
{
	switch (lvl)
	{
		#define X(sym, name, val, color) case CTB_LOG_##sym: return name;
		CTB_LOG_LEVELS(X)
		#undef X
		default: return "UNK";
	}
}

static const char* _ctb_log_get_color(int lvl)
{
	switch (lvl)
	{
		#define X(sym, name, val, color) case CTB_LOG_##sym: return color;
		CTB_LOG_LEVELS(X)
		#undef X
		default: return "";
	}
}

CTB_LOG_DEF void ctb_log_set_mask(unsigned int mask)
{
	_ctb_log_mask = mask;
}

CTB_LOG_DEF void ctb_log_enable(unsigned int flags)
{
	_ctb_log_mask |= flags;
}

CTB_LOG_DEF void ctb_log_disable(unsigned int flags)
{
	_ctb_log_mask &= ~flags;
}

CTB_LOG_DEF void ctb_log_printf(int lvl, const char* fmt, ...)
{
	if (lvl != CTB_LOG_NONE && !(_ctb_log_mask & lvl)) return;

	va_list args;
	va_start(args, fmt);

	const char* color = _ctb_log_get_color(lvl);

	if (lvl == CTB_LOG_NONE)
	{
		vprintf(fmt, args);
		printf(_CTB_COLOR_RESET "\n");
	}
	else if (lvl == CTB_LOG_CMD)
	{
		printf("%s[%s]: %s", color, _ctb_log_get_name(lvl), _CTB_COLOR_RESET);
		vprintf(fmt, args);
		printf(_CTB_COLOR_RESET "\n");
	}
	else
	{
		printf("%s[%s]: ", color, _ctb_log_get_name(lvl));
		vprintf(fmt, args);
		printf(_CTB_COLOR_RESET "\n");
	}
	va_end(args);
}

#endif /* CTB_LOG_IMPLEMENTATION */
