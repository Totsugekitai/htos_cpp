#ifndef HTOS_KERNEL_CONSOLE_HPP_
#define HTOS_KERNEL_CONSOLE_HPP_

#include <cstdint>

#include "graphics.hpp"

namespace console {
class Console {
public:
    static const uint32_t kRows = 25;
    static const uint32_t kColumns = 80;

    Console(graphics::Video& video);
    void PutString(const char *s);
private:
    graphics::Video& video_;
    char buffer_[kRows][kColumns + 1];
    uint32_t cursor_row_, cursor_column_;

    void Newline();
}; // class Console
} // namespace console

#endif
