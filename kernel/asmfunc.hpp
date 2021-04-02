#ifndef HTOS_KERNEL_ASMFUNC_HPP_
#define HTOS_KERNEL_ASMFUNC_HPP_

#include <cstdint>

extern "C" void IoOut8(uint16_t addr, uint8_t data);
extern "C" uint8_t IoIn8(uint16_t addr);
extern "C" void IoOut32(uint16_t addr, uint32_t data);
extern "C" uint32_t IoIn32(uint16_t addr);

#endif
