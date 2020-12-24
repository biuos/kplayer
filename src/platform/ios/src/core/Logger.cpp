
#include "Logger.h"
#include <cstdio>
#include <cstdarg>

NS_KT_START

std::mutex Logger::_s_mutex{};
Logger    *Logger::_s_instance{nullptr};

Logger& Logger::getLogger() {
    if(nullptr == _s_instance) {
        std::lock_guard<std::mutex> lock(_s_mutex);
        if(nullptr == _s_instance) {
            _s_instance = new Logger;
        }
    }
    return *_s_instance;
}

void Logger::init(void *priv) {
}

void Logger::logger( int level,
                     const char *tag,
                     const char *filename, const char *function, int line,
                     const char *message) {
    // todo iOS print the logs
}

void Logger::message( int level,
                      const char *tag,
                      const char *filename, const char *function, int line,
                      const char *fmt, ...) {
    static const size_t SIZE = 4096; // 4K
    try {
        char buffer[SIZE] = {0x00};
        va_list args;
        va_start(args, fmt);
        vsnprintf(buffer, SIZE, fmt, args);
        va_end(args);

        logger(level, tag, filename, function, line, buffer);
    } catch (...) {
    }
}


NS_KT_END
