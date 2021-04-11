#include <cstdint>

#include "piix4.hpp"

namespace {
uint8_t ReadAPICBASE(pci::Device& piix4) {
    uint32_t confreg = pci::ReadConfReg(piix4, 0x80);
    return (confreg & 0xff);
}
} // namespace

namespace piix4 {
pci::Device* GetPIIX4Pointer() {
    for (int i = 0; i < pci::devices.size(); ++i) {
        auto device = pci::devices[i];
        if (device.class_code.Match(0x06, 0x01, 0x00)) {
            return &pci::devices[i];
        }
    }
    return nullptr;
}

uint8_t ReadAPICBASEx(pci::Device& piix4) {
    uint8_t confreg = ReadAPICBASE(piix4);
    return (confreg & 0b00111111) >> 2;
}

uint8_t ReadAPICBASEy(pci::Device& piix4) {
    uint8_t confreg = ReadAPICBASE(piix4);
    return (confreg & 0b00000011) * 4;
}
} // namespace piix4
