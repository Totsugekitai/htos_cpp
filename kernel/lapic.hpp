#ifndef HTOS_KERNEL_LAPIC_HPP_
#define HTOS_KERNEL_LAPIC_HPP_

#include <cstdint>

namespace lapic {
uint64_t ReadIa32ApicBaseMsr();
} // namespace lapic

#endif
