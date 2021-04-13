#include <cstdint>

#include "pci.hpp"
#include "error.hpp"
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

error::Error AddDevice(const pci::Device& device) {
    if (pci::num_device == pci::devices.size()) {
        return error::MAKE_ERROR(error::Error::kFull);
    }
    pci::devices[pci::num_device] = device;
    ++pci::num_device;
    return error::MAKE_ERROR(error::Error::kSuccess);
}

error::Error ScanBus(uint8_t bus);

error::Error ScanFunction(uint8_t bus, uint8_t device, uint8_t function) {
    auto class_code = pci::ReadClassCode(bus, device, function);
    auto header_type = pci::ReadHeaderType(bus, device, function);
    pci::Device dev{bus, device, function, header_type, class_code};
    if (auto err = AddDevice(dev)) {
        return err;
    }

    if (class_code.Match(0x06u, 0x04u)) {
        // PCI-PCIブリッジの場合
        auto bus_numbers = pci::ReadBusNumbers(bus, device, function);
        uint8_t secondary_bus = (bus_numbers >> 8) & 0xffu;
        return ScanBus(secondary_bus);
    }

    return error::MAKE_ERROR(error::Error::kSuccess);
}

error::Error ScanDevice(uint8_t bus, uint8_t device) {
    if (auto err = ScanFunction(bus, device, 0)) {
        return err;
    }
    if (pci::IsSingleFunctionDevice(pci::ReadHeaderType(bus, device, 0))) {
        return error::MAKE_ERROR(error::Error::kSuccess);
    }

    for (uint8_t function = 1; function < 8; ++function) {
        if (pci::ReadVendorId(bus, device, function) == 0xffffu) {
            continue;
        }
        if (auto err = ScanFunction(bus, device, function)) {
            return err;
        }
    }

    return error::MAKE_ERROR(error::Error::kSuccess);
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

    return error::MAKE_ERROR(error::Error::kSuccess);
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

ClassCode ReadClassCode(uint8_t bus, uint8_t device, uint8_t function) {
    WriteAddress(MakeAddress(bus, device, function, 0x08));
    auto reg =  ReadData();
    ClassCode cc;
    cc.base = (reg >> 24) & 0xffu;
    cc.sub = (reg >> 16) & 0xffu;
    cc.interface = (reg >> 8) & 0xffu;
    return cc;
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

    return error::MAKE_ERROR(error::Error::kSuccess);
}

uint32_t ReadConfReg(const Device& dev, uint8_t reg_addr) {
    WriteAddress(MakeAddress(dev.bus, dev.device, dev.function, reg_addr));
    return ReadData();
}

void WriteConfReg(const Device& dev, uint8_t reg_addr, uint32_t value) {
    WriteAddress(MakeAddress(dev.bus, dev.device, dev.function, reg_addr));
    WriteData(value);
}

error::WithError<uint64_t> ReadBar(Device &device, unsigned int bar_index) {
    if (bar_index >= 6) {
        return {0, error::MAKE_ERROR(error::Error::kIndexOutOfRange)};
    }

    const auto addr = CalcBarAddress(bar_index);
    const auto bar = ReadConfReg(device, addr);

    // 32 bit address
    if ((bar & 4u) == 0) {
        return {bar, error::MAKE_ERROR(error::Error::kSuccess)};
    }

    // 64 bit address
    if (bar_index >= 5) {
        return {0, error::MAKE_ERROR(error::Error::kIndexOutOfRange)};
    }

    const auto bar_upper = ReadConfReg(device, addr + 4);

    return {
        bar | (static_cast<uint64_t>(bar_upper) << 32),
        error::MAKE_ERROR(error::Error::kSuccess)
    };
}
} // namespace pci
