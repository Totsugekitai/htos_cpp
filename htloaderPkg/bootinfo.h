#ifndef _BOOTINFO_H_
#define _BOOTINFO_H_

#include <stdint.h>

typedef struct pixel {
    uint8_t rgb[3];
    uint8_t _rsvd;
} __attribute__((packed)) pixel_t;

typedef enum pixel_format {
    RGBColor,
    BGRColor,
} pixel_format_t;

typedef struct videoinfo {
    pixel_format_t format;
    pixel_t *frame_buffer_base;
    uint64_t frame_buffer_size;
    uint32_t horizonal_resolution;
    uint32_t vertical_resolution;
    uint32_t pixels_per_scan_line;
} videoinfo_t;

typedef struct bootinfo {
    videoinfo_t vinfo;
} bootinfo_t;

void kernel_entry(bootinfo_t *binfo);

#endif
