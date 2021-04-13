#include <cstddef>
#include <cstdio>

#include "logger.hpp"
#include "console.hpp"

namespace {
logger::LogLevel log_level = logger::kWarn;
};

extern console::Console* con;

namespace logger {
void SetLogLevel(LogLevel level) {
    log_level = level;
}

int Log(LogLevel level, const char *fmt, ...) {
    if (level > log_level) {
        return 0;
    }

    va_list ap;
    int result;
    char s[1024];

    va_start(ap, fmt);
    result = vsprintf(s, fmt, ap);
    va_end(ap);

    con->PutString(s);
    return result;
}
} // namespace logger
