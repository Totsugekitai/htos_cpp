#ifndef HTOS_KERNEL_INTERRUPT_HPP_
#define HTOS_KERNEL_INTERRUPT_HPP_

#include <cstdint>
#include <array>

namespace interrupt {
enum DescriptorType {
    kLDT = 2,
    kTSSAvailable = 9,
    kTSSBusy = 11,
    kCallGate = 12,
    kInterruptGate = 14,
    kTrapGate = 15,
};

union InterruptDescriptorAttributes {
    uint16_t data;
    struct {
        uint16_t ist : 3;
        uint16_t : 5;
        uint16_t type : 4;
        uint16_t : 1;
        uint16_t dpl : 2;
        uint16_t p : 1;
    } __attribute__((packed)) bits;
} __attribute__((packed));

struct InterruptDescriptor {
    uint16_t offset_low;
    uint16_t segment_selector;
    InterruptDescriptorAttributes attr;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

void SetIDTEntry(InterruptDescriptor& desc, InterruptDescriptorAttributes attr, uint64_t offset, uint16_t segment_selector);
InterruptDescriptorAttributes MakeIDTAttr(DescriptorType type, uint8_t dpl);

struct InterruptFrame {
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
};

inline std::array<InterruptDescriptor, 256> IDT;

void IntHandlerFirstSerialPort(InterruptFrame* frame);

} // namespace interrupt

#endif
