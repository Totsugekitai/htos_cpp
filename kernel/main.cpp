#include <cstdint>

extern "C" void kernel_entry() {
    while (1) __asm__ volatile("hlt");
}
