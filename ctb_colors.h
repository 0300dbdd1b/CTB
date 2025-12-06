#ifndef _CTB_COLORS_H
#define _CTB_COLORS_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CTB_NO_COLORS)
    #define CTB_ANSI_RESET          ""
    #define CTB_ANSI_BOLD           ""
    #define CTB_ANSI_DIM            ""
    #define CTB_ANSI_ITALIC         ""
    #define CTB_ANSI_UNDERLINE      ""
    #define CTB_ANSI_BLINK          ""
    #define CTB_ANSI_REVERSE        ""
    #define CTB_ANSI_HIDDEN         ""

    #define CTB_ANSI_FG_BLACK       ""
    #define CTB_ANSI_FG_RED         ""
    #define CTB_ANSI_FG_GREEN       ""
    #define CTB_ANSI_FG_YELLOW      ""
    #define CTB_ANSI_FG_BLUE        ""
    #define CTB_ANSI_FG_MAGENTA     ""
    #define CTB_ANSI_FG_CYAN        ""
    #define CTB_ANSI_FG_WHITE       ""

    #define CTB_ANSI_BG_BLACK       ""
    #define CTB_ANSI_BG_RED         ""
    #define CTB_ANSI_BG_GREEN       ""
    #define CTB_ANSI_BG_YELLOW      ""
    #define CTB_ANSI_BG_BLUE        ""
    #define CTB_ANSI_BG_MAGENTA     ""
    #define CTB_ANSI_BG_CYAN        ""
    #define CTB_ANSI_BG_WHITE       ""

	#define CTB_ANSI_FG_BR_BLACK    ""
    #define CTB_ANSI_FG_BR_RED      ""
    #define CTB_ANSI_FG_BR_GREEN    ""
    #define CTB_ANSI_FG_BR_YELLOW   ""
    #define CTB_ANSI_FG_BR_BLUE     ""
    #define CTB_ANSI_FG_BR_MAGENTA  ""
    #define CTB_ANSI_FG_BR_CYAN     ""
    #define CTB_ANSI_FG_BR_WHITE    ""

	#define CTB_ANSI_BG_BR_BLACK    ""
    #define CTB_ANSI_BG_BR_RED      ""
    #define CTB_ANSI_BG_BR_GREEN    ""
    #define CTB_ANSI_BG_BR_YELLOW   ""
    #define CTB_ANSI_BG_BR_BLUE     ""
    #define CTB_ANSI_BG_BR_MAGENTA  ""
    #define CTB_ANSI_BG_BR_CYAN     ""
    #define CTB_ANSI_BG_BR_WHITE    ""

#else
	#define CTB_ANSI_RESET			"\x1b[0m"
    #define CTB_ANSI_BOLD			"\x1b[1m"
    #define CTB_ANSI_DIM			"\x1b[2m"
    #define CTB_ANSI_ITALIC			"\x1b[3m"
    #define CTB_ANSI_UNDERLINE		"\x1b[4m"
    #define CTB_ANSI_BLINK			"\x1b[5m"
    #define CTB_ANSI_REVERSE		"\x1b[7m"
    #define CTB_ANSI_HIDDEN			"\x1b[8m"

    #define CTB_ANSI_FG_BLACK       "\x1b[30m"
    #define CTB_ANSI_FG_RED         "\x1b[31m"
    #define CTB_ANSI_FG_GREEN       "\x1b[32m"
    #define CTB_ANSI_FG_YELLOW      "\x1b[33m"
    #define CTB_ANSI_FG_BLUE        "\x1b[34m"
    #define CTB_ANSI_FG_MAGENTA     "\x1b[35m"
    #define CTB_ANSI_FG_CYAN        "\x1b[36m"
    #define CTB_ANSI_FG_WHITE       "\x1b[37m"

    #define CTB_ANSI_BG_BLACK       "\x1b[40m"
    #define CTB_ANSI_BG_RED         "\x1b[41m"
    #define CTB_ANSI_BG_GREEN       "\x1b[42m"
    #define CTB_ANSI_BG_YELLOW      "\x1b[43m"
    #define CTB_ANSI_BG_BLUE        "\x1b[44m"
    #define CTB_ANSI_BG_MAGENTA     "\x1b[45m"
    #define CTB_ANSI_BG_CYAN        "\x1b[46m"
    #define CTB_ANSI_BG_WHITE       "\x1b[47m"

    #define CTB_ANSI_FG_BR_BLACK    "\x1b[90m"
    #define CTB_ANSI_FG_BR_RED      "\x1b[91m"
    #define CTB_ANSI_FG_BR_GREEN    "\x1b[92m"
    #define CTB_ANSI_FG_BR_YELLOW   "\x1b[93m"
    #define CTB_ANSI_FG_BR_BLUE     "\x1b[94m"
    #define CTB_ANSI_FG_BR_MAGENTA  "\x1b[95m"
    #define CTB_ANSI_FG_BR_CYAN     "\x1b[96m"
    #define CTB_ANSI_FG_BR_WHITE    "\x1b[97m"

    #define CTB_ANSI_BG_BR_BLACK    "\x1b[100m"
    #define CTB_ANSI_BG_BR_RED      "\x1b[101m"
    #define CTB_ANSI_BG_BR_GREEN    "\x1b[102m"
    #define CTB_ANSI_BG_BR_YELLOW   "\x1b[103m"
    #define CTB_ANSI_BG_BR_BLUE     "\x1b[104m"
    #define CTB_ANSI_BG_BR_MAGENTA  "\x1b[105m"
    #define CTB_ANSI_BG_BR_CYAN     "\x1b[106m"
    #define CTB_ANSI_BG_BR_WHITE    "\x1b[107m"

#endif

#if defined(_WIN32) || defined(_WIN64)
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
    #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif

    #define CTB_ENABLE_WINDOWS_ANSI \
        do { \
            HANDLE _hOut = GetStdHandle(STD_OUTPUT_HANDLE); \
            DWORD _dwMode = 0; \
            if (_hOut != INVALID_HANDLE_VALUE && GetConsoleMode(_hOut, &_dwMode)) { \
                _dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; \
                SetConsoleMode(_hOut, _dwMode); \
            } \
        } while(0)
#else
    #define CTB_ENABLE_WINDOWS_ANSI do {} while(0)
#endif


#define CTB_RGB_R(c)            (((c) >> 16) & 0xFF)
#define CTB_RGB_G(c)            (((c) >> 8)  & 0xFF)
#define CTB_RGB_B(c)            ((c) & 0xFF)
#define CTB_RGB_MAKE(r, g, b)   ((((r) & 0xFF) << 16) | (((g) & 0xFF) << 8) | ((b) & 0xFF))

#define CTB_RGB_BLACK           0x000000
#define CTB_RGB_WHITE           0xFFFFFF
#define CTB_RGB_GRAY            0x808080
#define CTB_RGB_SILVER          0xC0C0C0
#define CTB_RGB_DK_GRAY         0x404040

#define CTB_RGB_RED             0xFF0000
#define CTB_RGB_LIME            0x00FF00
#define CTB_RGB_BLUE            0x0000FF
#define CTB_RGB_YELLOW          0xFFFF00
#define CTB_RGB_CYAN            0x00FFFF
#define CTB_RGB_MAGENTA         0xFF00FF

#define CTB_RGB_MAROON          0x800000
#define CTB_RGB_OLIVE           0x808000
#define CTB_RGB_GREEN           0x008000
#define CTB_RGB_PURPLE          0x800080
#define CTB_RGB_TEAL            0x008080
#define CTB_RGB_NAVY            0x000080

#define CTB_RGB_ORANGE          0xFFA500
#define CTB_RGB_GOLD            0xFFD700
#define CTB_RGB_PINK            0xFFC0CB
#define CTB_RGB_HOTPINK         0xFF69B4
#define CTB_RGB_SALMON          0xFA8072
#define CTB_RGB_VIOLET          0xEE82EE
#define CTB_RGB_INDIGO          0x4B0082
#define CTB_RGB_TURQUOISE       0x40E0D0
#define CTB_RGB_SKYBLUE         0x87CEEB
#define CTB_RGB_BEIGE           0xF5F5DC
#define CTB_RGB_BROWN           0xA52A2A

#ifdef CTB_COLORS_NOPREFIX
    #define ANSI_RESET          CTB_ANSI_RESET
    #define ANSI_BOLD           CTB_ANSI_BOLD
    #define ANSI_UNDERLINE      CTB_ANSI_UNDERLINE

	#define ANSI_FG_BLACK       CTB_ANSI_FG_BLACK
    #define ANSI_FG_RED         CTB_ANSI_FG_RED
    #define ANSI_FG_GREEN       CTB_ANSI_FG_GREEN
    #define ANSI_FG_YELLOW      CTB_ANSI_FG_YELLOW
    #define ANSI_FG_BLUE        CTB_ANSI_FG_BLUE
    #define ANSI_FG_MAGENTA     CTB_ANSI_FG_MAGENTA
    #define ANSI_FG_CYAN        CTB_ANSI_FG_CYAN
    #define ANSI_FG_WHITE       CTB_ANSI_FG_WHITE
    
    #define ANSI_BG_BLACK       CTB_ANSI_BG_BLACK
    #define ANSI_BG_RED         CTB_ANSI_BG_RED

    #define RGB_R               CTB_RGB_R
    #define RGB_G               CTB_RGB_G
    #define RGB_B               CTB_RGB_B
    #define RGB_MAKE            CTB_RGB_MAKE

    #define RGB_BLACK           CTB_RGB_BLACK
    #define RGB_WHITE           CTB_RGB_WHITE
    #define RGB_RED             CTB_RGB_RED
    #define RGB_LIME            CTB_RGB_LIME
    #define RGB_BLUE            CTB_RGB_BLUE
    #define RGB_YELLOW          CTB_RGB_YELLOW
    #define RGB_CYAN            CTB_RGB_CYAN
    #define RGB_MAGENTA         CTB_RGB_MAGENTA
    #define RGB_GRAY            CTB_RGB_GRAY
    #define RGB_ORANGE          CTB_RGB_ORANGE
    #define RGB_PURPLE          CTB_RGB_PURPLE

    #define ENABLE_WINDOWS_ANSI CTB_ENABLE_WINDOWS_ANSI
#endif

#ifdef __cplusplus
}
#endif
#endif /* _CTB_COLORS_H */
