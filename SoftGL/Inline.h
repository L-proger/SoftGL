#ifndef INLINE_H
#define INLINE_H

#undef INLINE
#ifdef _WIN32
#ifdef _MSC_VER
#define INLINE __forceinline
#endif
#else
#ifdef __GNUC__
#define INLINE __attribute__((always_inline))
#endif
#endif

#ifndef INLINE
#define INLINE inline
#endif

#endif // INLINE_H
