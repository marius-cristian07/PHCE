#ifndef C_LOG_H
#define C_LOG_H

#include <cstdio>

// Logging the C way: with macros. The preprocessor takes a message that is switched
// off out of the text, before the compiler sees it.

#define C_LOG_LEVEL_DEBUG 0
#define C_LOG_LEVEL_INFO 1
#define C_LOG_LEVEL_WARNING 2
#define C_LOG_LEVEL_ERROR 3
#define C_LOG_LEVEL_NONE 4

// Messages below this level are switched off
#define C_LOG_LEVEL C_LOG_LEVEL_WARNING

// Prints the label, then the message like printf, then a new line.
#define C_LOG_PRINT(label, ...) \
    (std::printf("[C   " label "] "), std::printf(__VA_ARGS__), std::printf("\n"))

#if C_LOG_LEVEL <= C_LOG_LEVEL_DEBUG
#define C_LOG_DEBUG(...) C_LOG_PRINT("DEBUG", __VA_ARGS__)
#else
#define C_LOG_DEBUG(...)
#endif

#if C_LOG_LEVEL <= C_LOG_LEVEL_INFO
#define C_LOG_INFO(...) C_LOG_PRINT("INFO ", __VA_ARGS__)
#else
#define C_LOG_INFO(...)
#endif

// TODO 1: make C_LOG_WARNING work like the other three. Its label is "WARN ".
#if C_LOG_LEVEL <= C_LOG_LEVEL_WARNING
#define C_LOG_WARNING(...) C_LOG_PRINT("WARN ", __VA_ARGS__)
#else
#define C_LOG_WARNING(...)
#endif

#if C_LOG_LEVEL <= C_LOG_LEVEL_ERROR
#define C_LOG_ERROR(...) C_LOG_PRINT("ERROR", __VA_ARGS__)
#else
#define C_LOG_ERROR(...)
#endif

#endif  // C_LOG_H
