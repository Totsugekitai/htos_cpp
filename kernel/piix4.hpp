#ifndef HTOS_KERNEL_PIIX4_HPP_
#define HTOS_KERNEL_PIIX4_HPP_

#include <cstdint>

#include "pci.hpp"

namespace piix4 {
pci::Device* GetPIIX4Pointer();
uint8_t ReadAPICBASEx(pci::Device& piix4);
uint8_t ReadAPICBASEy(pci::Device& piix4);
} // namespace piix4

#endif
