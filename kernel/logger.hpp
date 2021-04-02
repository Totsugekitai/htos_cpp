#ifndef HTOS_KERNEL_LOGGER_HPP_
#define HTOS_KERNEL_LOGGER_HPP_

namespace logger {
enum LogLevel {
    kError = 3,
    kWarn  = 4,
    kInfo  = 6,
    kDebug = 7,
};

void SetLogLevel(LogLevel level);
int Log(LogLevel level, const char* fmt, ...);
} // namespace logger

#endif
