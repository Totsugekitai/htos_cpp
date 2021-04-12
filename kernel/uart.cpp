#include <cstdint>

#include "asmfunc.hpp"
#include "uart.hpp"
#include "error.hpp"
#include "ioapic.hpp"
#include "pic.hpp"

namespace {
const uint16_t kCom1 = 0x3f8;
const uint8_t kIrqCom1 = 4;
} // namespace

namespace uart {
error::Error Init() {
    pic::Disable();
    IoOut8(kCom1 + 1, 0);
    IoOut8(kCom1 + 3, 0x80);
    IoOut8(kCom1 + 0, 115200 / 115200); // 115200 baud rate
    IoOut8(kCom1 + 1, 0);
    IoOut8(kCom1 + 3, 0x03);
    IoOut8(kCom1 + 4, 0x0b);
    IoOut8(kCom1 + 1, 0x01);

    if (IoIn8(kCom1 + 5) == 0xffu) {
        return error::MAKE_ERROR(error::Error::kNotImplemented);
    }

    IoIn8(kCom1 + 2);
    IoIn8(kCom1 + 0);

    ioapic::Enable(kIrqCom1, 0);
    return error::MAKE_ERROR(error::Error::kSuccess);
}

uint8_t GetInterruptEnableRegister() {
    return IoIn8(kCom1 + 1);
}

void SetInterruptEnableRegister(uint8_t reg) {
    IoOut8(kCom1 + 1, reg);
}

bool IsTransmitterHoldingRegister() {
    return IoIn8(kCom1 + 5) & 0x20;
}

bool IsDataReady() {
    return IoIn8(kCom1 + 5) & 1;
}

char getc() {
    while (!IsDataReady());
    return IoIn8(kCom1);
}

void putc(char c) {
    while (!IsTransmitterHoldingRegister());
    IoOut8(kCom1, c);
}

int puts(const char* fmt, ...) {
    va_list ap;
    int result;
    char s[1024];

    va_start(ap, fmt);
    result = vsprintf(s, fmt, ap);
    va_end(ap);

    for (char* p = s; *p != '\0'; ++p) {
        putc(*p);
    }
    putc('\0');

    return result;
}
} // namespace uart
