#ifndef HTOS_KERNEL_FONT_HPP_
#define HTOS_KERNEL_FONT_HPP_

#include <cstdint>

#include "graphics.hpp"

namespace font {
const uint8_t* GetFont(char c);
void WriteAscii(graphics::Video& video, uint32_t x, uint32_t y, char c);
void WriteString(graphics::Video& video, uint32_t x, uint32_t y, const char* s);
} // namespace font

#endif
