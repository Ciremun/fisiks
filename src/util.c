#include "util.h"

#ifdef __wasm__

extern int grid_size;
extern unsigned char __heap_base;

char *heap = (char *)&__heap_base;

static uint32_t SWAPS(uint32_t r)
{
    uint32_t ret = (r & 0xFF) << 24;
    r >>= 8;
    ret |= (r & 0xff) << 16;
    r >>= 8;
    ret |= (r & 0xff) << 8;
    r >>= 8;
    ret |= (r & 0xff) << 0;
    return ret;
}

u64 fisiks_strlen(const char *s)
{
    u64 sz = 0;
    while (s[sz] != '\0')
        sz++;
    return sz;
}

void *fisiks_memset(void *dest, int val, u64 len)
{
    unsigned char *ptr = dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}

void *fisiks_memcpy(void *dst, void const *src, u64 size)
{
    unsigned char *source = (unsigned char *)src;
    unsigned char *dest = (unsigned char *)dst;
    while (size--)
        *dest++ = *source++;
    return dst;
}

void *fisiks_malloc(unsigned long long size)
{
    heap += size;
    return heap - size;
}

void *fisiks_realloc(void *old_mem, unsigned long long size)
{
// since we only have a grid
    u64 old_size = GRID_SIZE(grid_size);
    if (size <= old_size) {
        heap -= old_size;
        return heap;
    }
    void *new_mem = fisiks_malloc(size);
    fisiks_memcpy(new_mem, old_mem, old_size);
    return new_mem;
}
#else
void print(double idebug)
{
    (void)idebug;
}
#endif // __wasm__
