#include <cstdint>

#include "bootinfo.hpp"
#include "graphics.hpp"
#include "font.hpp"

extern "C" void kernel_entry(const boot::bootinfo_t& binfo) {
    graphics::VIDEO.set(binfo.vinfo);
    graphics::VIDEO.FillRectangle(0, 0,
                                  graphics::VIDEO.GetHorizonalResolution(),
                                  graphics::VIDEO.GetVerticalResolution(),
                                  {{0x00, 0x00, 0x00}, 0x00});
    font::WriteAscii(graphics::VIDEO, 0, 0, 'C');
    while (1) __asm__ volatile("hlt");
}
