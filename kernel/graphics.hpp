#ifndef HTOS_KERNEL_GRAPHICS_HPP_
#define HTOS_KERNEL_GRAPHICS_HPP_

#include <cstdint>

#include "bootinfo.hpp"

namespace graphics {
class Video {
public:
    void set(const boot::videoinfo_t& vinfo);
    void WritePixel(uint32_t x, uint32_t y, const boot::pixel_t& color);
    void FillRectangle(uint32_t x, uint32_t y, uint32_t xlen, uint32_t ylen, const boot::pixel_t& color);
private:
    boot::videoinfo_t vinfo_;
};
inline Video VIDEO;
} // namespace graphics

#endif
