#include <cstdint>

#include "bootinfo.hpp"
#include "graphics.hpp"

namespace graphics {
void Video::set(const boot::videoinfo_t& vinfo) {
    this->vinfo_ = vinfo;
}

void Video::WritePixel(uint32_t x, uint32_t y, const boot::pixel_t& color) {
    volatile boot::pixel_t *p = this->vinfo_.frame_buffer_base + (y * this->vinfo_.horizonal_resolution) + x;
    p->rgb[0] = color.rgb[0];
    p->rgb[1] = color.rgb[1];
    p->rgb[2] = color.rgb[2];
    p->_rsvd = color._rsvd;
}

void Video::FillRectangle(uint32_t x, uint32_t y, uint32_t xlen, uint32_t ylen, const boot::pixel_t& color) {
    for (uint32_t dy = 0; dy < ylen; ++dy) {
        for (uint32_t dx = 0; dx < xlen; ++dx) {
            WritePixel(x + dx, y + dy, color);
        }
    }
}

uint32_t Video::GetHorizonalResolution() {
    return this->vinfo_.horizonal_resolution;
}

uint32_t Video::GetVerticalResolution() {
    return this->vinfo_.vertical_resolution;
}
} // namespace graphics
