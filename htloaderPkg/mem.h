#ifndef _MEM_H_
#define _MEM_H_

#include <stdint.h>

void *memcpy(void *dst, const void *src, uint64_t n);
void *memset(void *buf, int ch, uint64_t n);

#endif
