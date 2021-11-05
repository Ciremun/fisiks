#ifndef _UTIL_H_
#define _UTIL_H_

#ifdef __wasm__
#include <stdint.h>
#endif // __wasm__

#include "typedef.h"

#define GRID_SIZE(gs) (sizeof(int) * gs * gs)
#ifdef __wasm__
#define EXPORT(s) __attribute__((export_name(s)))
#else
#define EXPORT(s)
#endif // __wasm__

#ifdef __wasm__
#define malloc fisiks_malloc
#define realloc fisiks_realloc
#define strlen fisiks_strlen
#define memset fisiks_memset
#define memcpy fisiks_memcpy
#endif // __wasm__

#ifdef __wasm__
double OGGetAbsoluteTime();
static uint32_t SWAPS(uint32_t r);
u64 fisiks_strlen(const char *s);
void *fisiks_memset(void *dest, int val, u64 len);
void *fisiks_memcpy(void *dst, void const *src, u64 size);
void *fisiks_malloc(unsigned long long size);
void *fisiks_realloc(void *old_mem, unsigned long long size);
void print(double idebug);
#endif // __wasm__

void draw_text(const char *text, int x, int y, int font_size);

#endif // _UTIL_H_
