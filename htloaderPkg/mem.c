#include <stdint.h>

void *memcpy(void *dst, const void *src, uint64_t n)
{
    volatile uint8_t *d = dst;
    for (uint64_t i = 0; i < n; i++) {
        d[i] = ((const uint8_t *)src)[i];
    }
    return dst;
}

void *memset(void *buf, int ch, uint64_t n)
{
    volatile uint8_t *b = buf;
    char c = (char)ch;
    for (uint64_t i = 0; i < n; i++) {
        b[i] = c;
    }
    return (void *)b;
}
