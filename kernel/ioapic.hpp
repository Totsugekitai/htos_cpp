#ifndef HTOS_KERNEL_IOAPIC_HPP_
#define HTOS_KERNEL_IOAPIC_HPP_

#include <cstdint>

namespace {
struct IoApic {
    uint32_t reg;
    uint32_t _pad[3];
    uint32_t data;
};
}

namespace ioapic {
void Write(uint32_t reg, uint32_t data);
void Enable(int irq, int cpu_number);
} // namespace ioapic

#endif
