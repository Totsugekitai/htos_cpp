#include <cstdint>
#include <cstring>

#include "font.hpp"
#include "console.hpp"

namespace console {
Console::Console(graphics::Video& video)
    : video_(video), buffer_(), cursor_row_(0), cursor_column_(0) {
}

void Console::PutString(const char *s) {
    for (int i = 0; s[i] != '\0'; ++i) {
        if (s[i] == '\n') {
            Newline();
        } else {
            font::WriteAscii(video_, cursor_column_ * 8, cursor_row_ * 16, s[i]);
            buffer_[cursor_row_][cursor_column_] = s[i];
            ++cursor_column_;
        }
    }
}

void Console::Newline() {
    cursor_column_ = 0;
    if (cursor_row_ < kRows - 1) {
        ++cursor_row_;
    } else {
        for (int row = 0; row < kRows - 1; ++row) {
            memcpy(buffer_[row], buffer_[row + 1], kColumns + 1);
            font::WriteString(video_, 0, row * 16, buffer_[row]);
        }
        memset(buffer_[kRows - 1], 0, kColumns + 1);
    }
}
} // namespace console
