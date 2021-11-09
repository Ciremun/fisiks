#ifndef _UTIL_H_
#define _UTIL_H_

#ifdef __wasm__
#include <stdint.h>
#endif // __wasm__

#include "typedef.h"

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif // min

#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif // min

#ifdef __wasm__
#define EXPORT(s) __attribute__((export_name(s)))
#else
#define EXPORT(s)
#endif // __wasm__

#ifdef __wasm__
#define malloc fisiks_malloc
#define calloc fisiks_calloc
#define realloc fisiks_realloc
#define strlen fisiks_strlen
#define memset fisiks_memset
#define memcpy fisiks_memcpy
#endif // __wasm__

#ifdef __wasm__
double OGGetAbsoluteTime();
u64 fisiks_strlen(const char *s);
void *fisiks_memset(void *dest, int val, u64 len);
void *fisiks_memcpy(void *dst, void const *src, u64 size);
void *fisiks_malloc(u64 size);
void *fisiks_calloc(u64 num, u64 size);
void *fisiks_realloc(void *old_mem, u64 size);
void print(double idebug);
#endif // __wasm__

void draw_text(const char *text, int x, int y, int font_size);

typedef struct {
    u64 length;
    char *content;
} String;

#endif // _UTIL_H_
