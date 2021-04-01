#ifndef HTOS_KERNEL_PCI_HPP_
#define HTOS_KERNEL_PCI_HPP_

#include <cstdint>

namespace pci {
uint32_t MakeAddress(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg_addr);
void WriteAddress(uint32_t addr);
void WriteData(uint32_t data);
uint32_t ReadData();
uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function);

const uint16_t kConfigAddress = 0x0cf8;
const uint16_t kConfigData = 0x0cfc;
} // namespace pci

#endif
