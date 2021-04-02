#include <cstdint>

#include "ioapic.hpp"

namespace {
volatile struct IoApic* IoApic = (volatile struct IoApic*)0xfec00000;
const uint32_t kRegTable = 0x10;
const uint32_t kIRQ0 = 32;
} // namespace

namespace ioapic {
void Write(uint32_t reg, uint32_t data) {
    IoApic->reg = reg;
    IoApic->data = data;
}

void Enable(int irq, int cpu_number) {
    Write(kRegTable + 2 * irq    , kIRQ0 + irq);
    Write(kRegTable + 2 * irq + 1, cpu_number << 24);
}
} // namespace ioapic
