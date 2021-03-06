#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "bootinfo.hpp"
#include "graphics.hpp"
#include "font.hpp"
#include "console.hpp"
#include "pci.hpp"
#include "logger.hpp"
#include "lapic.hpp"
#include "ioapic.hpp"
#include "interrupt.hpp"
#include "asmfunc.hpp"
#include "uart.hpp"

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
    logger::SetLogLevel(logger::kDebug);

    // PCIのスキャン
    auto err = pci::ScanAllBus();
    logger::Log(logger::kDebug, "ScanAllBus: %s\n", err.Name());
    for (int i = 0; i < pci::num_device; ++i) {
        const auto& dev = pci::devices[i];
        auto vendor_id = pci::ReadVendorId(dev.bus, dev.device, dev.function);
        auto class_code = pci::ReadClassCode(dev.bus, dev.device, dev.function);
        logger::Log(logger::kDebug, "%d.%d.%d: vend %04x, class %02x,%02x,%02x, head %02x\n",
                    dev.bus, dev.device, dev.function,
                    vendor_id, class_code.base, class_code.sub, class_code.interface,
                    dev.header_type);
    }

    // LAPICのデバッグプリント
    auto apic_base_msr = lapic::ReadIa32ApicBaseMsr();
    logger::Log(logger::kDebug, "base_msr: %lx\n", apic_base_msr);
    if (apic_base_msr & 0b100000000000u) logger::Log(logger::kDebug, "lapic enable\n");

    // IOAPICのデバッグプリント
    auto ioapic_err = ioapic::SetIOAPICAddress();
    logger::Log(logger::kDebug, "[%s] I/O APIC Base Addess: 0x%x\n",
                ioapic_err.error.Name(),
                ioapic_err.value);

    // IDTの設定
    const uint16_t cs = GetCS();
    interrupt::SetIDTEntry(interrupt::IDT[32+4], interrupt::MakeIDTAttr(interrupt::DescriptorType::kInterruptGate, 0), reinterpret_cast<uint64_t>(interrupt::IntHandlerFirstSerialPort), cs);
    LoadIDT(sizeof(interrupt::IDT) - 1, reinterpret_cast<uintptr_t>(&interrupt::IDT[0]));
    logger::Log(logger::kDebug, "IDT Setting done.\n");

    logger::Log(logger::kDebug, "[%s] UART init.\n", uart::Init().Name());

    while (1) __asm__ volatile("sti; hlt");
}
