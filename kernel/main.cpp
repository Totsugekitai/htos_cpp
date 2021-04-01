#include <cstdint>

#include "bootinfo.hpp"
#include "graphics.hpp"

extern "C" void kernel_entry(const boot::bootinfo_t& binfo) {
    
    while (1) __asm__ volatile("hlt");
}
