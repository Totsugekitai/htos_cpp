#include <cstdint>

#include "asmfunc.hpp"

namespace lapic {
uint64_t ReadIa32ApicBaseMsr() {
    return (static_cast<uint64_t>(ReadIa32ApicBaseMsrHigh()) << 32) | ReadIa32ApicBaseMsrLow();
}
} // namespace lapic
