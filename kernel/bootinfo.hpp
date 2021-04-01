#ifndef HTOS_KERNEL_BOOTINFO_HPP_
#define HTOS_KERNEL_BOOTINFO_HPP_

#include <cstdint>

namespace boot {
typedef struct {
    uint8_t rgb[3];
    uint8_t _rsvd;
} __attribute__((packed)) pixel_t;

typedef enum {
    RGBColor,
    BGRColor,
} pixel_format_t;

typedef struct {
    pixel_format_t format;
    pixel_t *frame_buffer_base;
    uint64_t frame_buffer_size;
    uint32_t horizonal_resolution;
    uint32_t vertical_resolution;
    uint32_t pixels_per_scan_line;
} videoinfo_t;

typedef struct {
    videoinfo_t vinfo;
} bootinfo_t;
} // namespace boot

#endif
