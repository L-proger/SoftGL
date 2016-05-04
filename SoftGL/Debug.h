#ifndef Debug_h__
#define Debug_h__

#include <cstdio>
#include <cassert>

#if defined(DEBUG) || defined(_DEBUG)
#define SGL_DEBUG
#define SGL_ASSERT(__expr, __text) assert(__expr && __text);
#else
#define SGL_ASSERT(__expr, __text)
#endif

#define DebugInfo(...) printf("Info: "); printf(__VA_ARGS__); printf("\n");
#define DebugError(...)  printf("Error: "); printf(__VA_ARGS__); printf("\n");

#endif // Debug_h__
