#include <cstdint>

#include "bootinfo.hpp"
#include "graphics.hpp"

extern "C" void kernel_entry(const boot::bootinfo_t& binfo) {
    graphics::VIDEO.set(binfo.vinfo);
    graphics::VIDEO.FillRectangle(10, 10, 100, 100, {{0xff, 0xff, 0xff}, 0x00});
    while (1) __asm__ volatile("hlt");
}
