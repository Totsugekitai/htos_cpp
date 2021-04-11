#include <cstdint>

#include "logger.hpp"
#include "interrupt.hpp"

namespace {
void NotifyEndOfInterrupt() {
    volatile auto end_of_interrupt = reinterpret_cast<uint32_t*>(0xfee000b0);
    *end_of_interrupt = 0;
}
} // namespace

namespace interrupt {
void SetIDTEntry(InterruptDescriptor& desc, InterruptDescriptorAttributes attr, uint64_t offset, uint16_t segment_selector) {
    desc.attr = attr;
    desc.offset_low = offset & 0xffffu;
    desc.offset_mid = (offset >> 16) & 0xffffu;
    desc.offset_high = offset >> 32;
    desc.segment_selector = segment_selector;
}
InterruptDescriptorAttributes MakeIDTAttr(DescriptorType type, uint8_t dpl) {
    InterruptDescriptorAttributes attr;
    attr.bits.type = type;
    attr.bits.dpl = dpl;
    attr.bits.ist = 0;
    attr.bits.p = 1;
    return attr;
}

__attribute__((interrupt))
void IntHandlerFirstSerialPort(InterruptFrame* frame) {
    logger::Log(logger::kDebug, "Serial interrupt\n");
    NotifyEndOfInterrupt();
}
} // namespace interrupt
