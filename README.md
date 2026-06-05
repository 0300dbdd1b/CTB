
# CTB

`CTB` is a small side project of mine: a personal C toolbox made of reusable stb-style headers, macros, types, and small utilities that I use across my own C projects.

The goal is not to be a polished framework, but to keep common C helpers in one place so I can reuse them without rewriting the same code every time.

> Status: experimental. The API is not stable yet.

## What is inside

CTB currently contains utilities for:

-   platform, compiler, architecture, and feature detection;
    
-   common C macros;
    
-   fixed-width integer aliases and small vector types;
    
-   arena allocation;
    
-   dynamic arrays;
    
-   string helpers;
    
-   ANSI colors and RGB helpers;
    
-   simple logging;
    
-   hash functions such as SHA, HMAC, RIPEMD160, and PBKDF2;
    
-   matrix helpers;
    
-   early filesystem helpers.
    

Some modules are more complete than others. This repository is still evolving as I extract and clean up code from my own projects.

## Usage

Most modules follow the stb-style pattern.

Include the header normally where you need declarations:

```c
#include "ctb.h"

```

In one translation unit, define the implementation macro before including it:

```c
#define CTB_IMPLEMENTATION
#include "ctb.h"

```

You can also include individual headers directly when you only need a specific module.

## Prefixes

By default, CTB uses the `ctb_` / `CTB_` prefix to avoid polluting the global namespace.

For personal projects where shorter names are preferred, some modules support no-prefix mode:

```c
#define CTB_NOPREFIX
#include "ctb.h"

```

## Notes

This is mainly a personal toolbox. It may contain rough edges, unfinished modules, and APIs that change as I use it in real projects.

Use it as source code to read, copy, adapt, or build from, not as a stable dependency yet.

## License

This project uses the [UNO Reverse Public License](https://github.com/Zuhaitz-dev/URPL).
