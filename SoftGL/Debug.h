#ifndef Debug_h__
#define Debug_h__

#include <cstdio>

#define DebugInfo(...) printf("Info: "); printf(__VA_ARGS__); printf("\n");
#define DebugError(...)  printf("Error: "); printf(__VA_ARGS__); printf("\n");

#endif // Debug_h__
