#ifndef HTOS_KERNEL_IOAPIC_HPP_
#define HTOS_KERNEL_IOAPIC_HPP_

#include <cstdint>

#include "error.hpp"

namespace {
struct IoApic {
    uint32_t reg;
    uint32_t _pad[3];
    uint32_t data;
} __attribute__((packed));
}

namespace ioapic {
error::WithError<uintptr_t> SetIOAPICAddress();
uint32_t Read(uint32_t reg);
void Write(uint32_t reg, uint32_t data);
void Enable(uint32_t irq, uint32_t cpu_number);
} // namespace ioapic

#endif
