#include <cstdint>

#include "console.hpp"

namespace console {
Console::Console(graphics::Video& video, boot::pixel_t char_color, boot::pixel_t bg_color)
    : video_(video), char_color_(char_color), bg_color_(bg_color),
      buffer_(), cursor_row_(0), cursor_column_(0) {
}

    void Console::PutString(const char *s) {
        for (int i = 0; s[i] != '\0'; ++i) {
            if (s[i] == '\n') {
                Newline();
            } else {
                
            }
        }
    }
} // namespace console
