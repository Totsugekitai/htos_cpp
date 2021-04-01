#include <cstdint>

#include "pci.hpp"
#include "asmfunc.hpp"

namespace {
uint32_t MakeAddress(uint8_t bus, uint8_t device, uint8_t function, uint8_t reg_addr) {
    auto shl = [](uint32_t x, unsigned int bits) {
        return x << bits;
    };

    return shl(1, 31)
        | shl(bus, 16)
        | shl(device, 11)
        | shl(function, 8)
        | (reg_addr & 0b11111100);
}

error::Error AddDevice(uint8_t bus, uint8_t device, uint8_t function, uint8_t header_type) {
    if (pci::num_device == pci::devices.size()) {
        return error::Error::kFull;
    }
    pci::devices[pci::num_device] = pci::Device{bus, device, function, header_type};
    ++pci::num_device;
    return error::Error::kSuccess;
}

error::Error ScanBus(uint8_t bus);

error::Error ScanFunction(uint8_t bus, uint8_t device, uint8_t function) {
    auto header_type = pci::ReadHeaderType(bus, device, function);
    if (auto err = AddDevice(bus, device, function, header_type)) {
        return err;
    }

    auto class_code = pci::ReadClassCode(bus, device, function);
    uint8_t base = (class_code >> 24) & 0xffu;
    uint8_t sub = (class_code >> 16) & 0xffu;

    if (base == 0x06u && sub == 0x04u) {
        // PCI-PCIブリッジの場合
        auto bus_numbers = pci::ReadBusNumbers(bus, device, function);
        uint8_t secondary_bus = (bus_numbers >> 8) & 0xffu;
        return ScanBus(secondary_bus);
    }

    return error::Error::kSuccess;
}

error::Error ScanDevice(uint8_t bus, uint8_t device) {
    if (auto err = ScanFunction(bus, device, 0)) {
        return err;
    }
    if (pci::IsSingleFunctionDevice(pci::ReadHeaderType(bus, device, 0))) {
        return error::Error::kSuccess;
    }

    for (uint8_t function = 1; function < 8; ++function) {
        if (pci::ReadVendorId(bus, device, function) == 0xffffu) {
            continue;
        }
        if (auto err = ScanFunction(bus, device, function)) {
            return err;
        }
    }

    return error::Error::kSuccess;
}

error::Error ScanBus(uint8_t bus) {
    for (uint8_t device = 0; device < 32; ++device) {
        if (pci::ReadVendorId(bus, device, 0) == 0xffffu) {
            continue;
        }
        if (auto err = ScanDevice(bus, device)) {
            return err;
        }
    }

    return error::Error::kSuccess;
}
} // namespace

namespace pci {
void WriteAddress(uint32_t addr) {
    IoOut32(kConfigAddress, addr);
}

void WriteData(uint32_t data) {
    IoOut32(kConfigData, data);
}

uint32_t ReadData() {
    return IoIn32(kConfigData);
}

uint16_t ReadVendorId(uint8_t bus, uint8_t device, uint8_t function) {
    WriteAddress(MakeAddress(bus, device, function, 0x00));
    return ReadData() & 0xffffu;
}

uint16_t ReadDeviceId(uint8_t bus, uint8_t device, uint8_t function) {
    WriteAddress(MakeAddress(bus, device, function, 0x00));
    return ReadData() >> 16;
}

uint8_t ReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
    WriteAddress(MakeAddress(bus, device, function, 0x0c));
    return (ReadData() >> 16) & 0xffu;
}

uint32_t ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    WriteAddress(MakeAddress(bus, device, function, 0x08));
    return ReadData();
}

uint32_t ReadBusNumbers(uint8_t bus, uint8_t device, uint8_t function) {
    WriteAddress(MakeAddress(bus, device, function, 0x18));
    return ReadData();
}

bool IsSingleFunctionDevice(uint8_t header_type) {
    return (header_type & 0x80u) == 0;
}

error::Error ScanAllBus() {
    num_device = 0;

    auto header_type = ReadHeaderType(0, 0, 0);
    if (IsSingleFunctionDevice(header_type)) {
        return ScanBus(0);
    }

    for (uint8_t function = 1; function < 8; ++function) {
        if (ReadVendorId(0, 0, function) == 0xffffu) {
            continue;
        }
        if (auto err = ScanBus(function)) {
            return err;
        }
    }

    return error::Error::kSuccess;
}
} // namespace pci
