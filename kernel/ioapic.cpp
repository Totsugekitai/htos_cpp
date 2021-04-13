#include <cstdint>

#include "error.hpp"
#include "ioapic.hpp"
#include "piix4.hpp"

namespace {
volatile struct IoApic* IoApic = (volatile struct IoApic*)0xfec00000ull;
const uint32_t kRedTable = 0x10;
const uint32_t kIRQ0 = 32;
} // namespace

namespace ioapic {
error::WithError<uintptr_t> SetIOAPICAddress() {
    auto piix4 = piix4::GetPIIX4Pointer();
    if (piix4 == nullptr) {
        return {
            0,
            error::MAKE_ERROR(error::Error::kEmpty)
        };
    }
    auto x = piix4::ReadAPICBASEx(*piix4);
    auto y = piix4::ReadAPICBASEy(*piix4);
    IoApic = (volatile struct IoApic*)(0xfec00000ull + (x << 16) + (y << 12));
    return {
        reinterpret_cast<uintptr_t>(IoApic),
        error::MAKE_ERROR(error::Error::kSuccess)
    };
}

uint32_t Read(uint32_t reg) {
    IoApic->reg = reg;
    return IoApic->data;
}

void Write(uint32_t reg, uint32_t data) {
    IoApic->reg = reg;
    IoApic->data = data;
}

void Enable(uint32_t irq, uint32_t cpu_number) {
    Write(kRedTable + 2 * irq    , kIRQ0 + irq);
    Write(kRedTable + 2 * irq + 1, cpu_number << 24);
}
} // namespace ioapic
