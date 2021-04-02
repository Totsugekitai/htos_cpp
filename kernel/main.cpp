#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "bootinfo.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"
#include "pci.hpp"
#include "logger.hpp"

char console_buf[sizeof(console::Console)];
console::Console* con;

//void* operator new(size_t size, void* buf) { return buf; }
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

    con = new(console_buf) console::Console{graphics::VIDEO};

    printk("Welcome to HTOS!\n");
    logger::SetLogLevel(logger::kWarn);

    // PCIのスキャン
    auto err = pci::ScanAllBus();
    logger::Log(logger::kDebug, "ScanAllBus: %s\n", err.Name());
    for (int i = 0; i < pci::num_device; ++i) {
        const auto& dev = pci::devices[i];
        auto vendor_id = pci::ReadVendorId(dev.bus, dev.device, dev.function);
        auto class_code = pci::ReadClassCode(dev.bus, dev.device, dev.function);
        logger::Log(logger::kDebug, "%d.%d.%d: vend %04x, class %08x, head %02x\n",
               dev.bus, dev.device, dev.function,
               vendor_id, class_code, dev.header_type);
    }

    while (1) __asm__ volatile("hlt");
}
