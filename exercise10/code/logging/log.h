#ifndef LOG_H
#define LOG_H

#include <cstdio>
#include <string>
#include <string_view>

// Logging the C++ way: with a function template. The level is a template argument, so
// the compiler knows it and can leave a message that is switched off out of the program.
namespace logging
{
    // From least to most serious
    enum class Severity
    {
        Debug,
        Info,
        Warning,
        Error,
        None
    };

    // Messages below this level are switched off
    constexpr Severity ACTIVE_LEVEL {Severity::Debug};

    constexpr std::string_view label(Severity level)
    {
        switch (level)
        {
            case Severity::Debug:
                return {"DEBUG"};
            case Severity::Info:
                return {"INFO "};
            case Severity::Warning:
                return {"WARN "};
            case Severity::Error:
                return {"ERROR"};
            default:
                return {"NONE "};
        }
    }

    // Prints the label, then the message like printf, then a new line.
    template <Severity Level, typename... Args>
    void log(std::string_view format, Args... args)
    {
        // TODO 2: only print if Level is ACTIVE_LEVEL or more serious. Decide it at
        //         compile time.
        // %.*s prints a string_view: first its size, then its characters
        const std::string_view name {label(Level)};
        std::printf("[C++ %.*s] ", static_cast<int>(name.size()), name.data());

        // printf wants a format that ends with '\0'. A std::string has one.
        const std::string text {format};
        std::printf(text.c_str(), args...);
        std::printf("\n");
    }
}  // namespace logging

#endif  // LOG_H
