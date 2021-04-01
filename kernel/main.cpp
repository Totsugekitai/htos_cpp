#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "bootinfo.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"

char console_buf[sizeof(console::Console)];
console::Console* con;

void* operator new(size_t size, void* buf) { return buf; }
void operator delete(void* obj) noexcept {}

int printk(const char *fmt, ...) {
    va_list ap;
    int result;
    char s[1024];

    va_start(ap, fmt);
    result = vsprintf(s, fmt, ap);
    va_end(ap);

    con->PutString(s);
    return result;
}

__attribute__((section(".text.entry")))
extern "C" void kernel_entry(const boot::bootinfo_t& binfo) {
    graphics::VIDEO.set(binfo.vinfo);
    graphics::VIDEO.FillRectangle(0, 0,
                                  graphics::VIDEO.GetHorizonalResolution(),
                                  graphics::VIDEO.GetVerticalResolution(),
                                  {{0x00, 0x00, 0x00}, 0x00});

    con = new(console_buf) console::Console(graphics::VIDEO, {{0xff, 0xff, 0xff}, 0x00}, {{0x00, 0x00, 0x00}, 0x00});

    for (int i = 0; i < 24; ++i) {
        printk("printk: %d\n", i);
    }

    while (1) __asm__ volatile("hlt");
}
