#include <cstdint>

#include "bootinfo.hpp"
#include "graphics.hpp"

extern const uint8_t fonts[0x100][16];

namespace font {
    const uint8_t* GetFont(char c) {
        return reinterpret_cast<const uint8_t*>(fonts[(int)c]);
    }

    void WriteAscii(graphics::Video& video, uint32_t x, uint32_t y, char c) {
        const uint8_t* font = GetFont(c);
        for (int dy = 0; dy < 16; ++dy) {
            for (int dx = 0; dx < 8; ++dx) {
                if (font[dy] << dx & 0b10000000) {
                    graphics::VIDEO.WritePixel(x + dx, y + dy, {{0xff, 0xff, 0xff}, 0x00});
                } else {
                    graphics::VIDEO.WritePixel(x + dx, y + dy, {{0x00, 0x00, 0x00}, 0x00});
                }
            }
        }
    }

} // namespace font
