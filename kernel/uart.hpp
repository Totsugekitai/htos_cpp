#ifndef HTOS_KERNEL_UART_HPP_
#define HTOS_KERNEL_UART_HPP_

#include <cstdint>

#include "error.hpp"

namespace uart {
error::Error Init();
uint8_t GetInterruptEnableRegister();
void SetInterruptEnableRegister(uint8_t reg);
char getc();
void putc(char c);
int puts(const char* fmt, ...);
} // namespace uart

#endif
