#include "pic.hpp"
#include "asmfunc.hpp"

namespace pic {
void Disable() {
    IoOut8(0xa1, 0xff);
    IoOut8(0x21, 0xff);
}
} // namespace pci
