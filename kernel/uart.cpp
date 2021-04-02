#include <cstdint>

#include "asmfunc.hpp"
#include "uart.hpp"
#include "error.hpp"
#include "ioapic.hpp"

namespace {
const uint16_t kCom1 = 0x3f8;
const uint8_t kIrqCom1 = 4;

    
} // namespace

namespace uart {
    error::Error Init() {
        IoOut8(kCom1 + 2, 0);
        IoOut8(kCom1 + 3, 0x80);
        IoOut8(kCom1 + 0, 115200 / 115200); // 115200 baud rate
        IoOut8(kCom1 + 1, 0);
        IoOut8(kCom1 + 3, 0x03);
        IoOut8(kCom1 + 4, 0);
        IoOut8(kCom1 + 1, 0x01);

        if (IoIn8(kCom1 + 5) == 0xffu) {
            return error::MAKE_ERROR(error::Error::kNotImplemented);
        }

        IoIn8(kCom1 + 2);
        IoIn8(kCom1 + 0);

        ioapic::Enable(kIrqCom1, 0);
    }
} // namespace uart
