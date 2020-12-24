
#include "Logger.h"
#include <cstdio>
#include <cstdarg>

#include <android/log.h>

NS_KT_START

static const char *default_tag = "ktplayer";

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
    const char *android_tag = tag;
    if( !android_tag ) android_tag = default_tag;
    int prio = ANDROID_LOG_UNKNOWN;
    switch (level) {
    case VERBOSE:
        prio = ANDROID_LOG_VERBOSE;
        break;
    case DEBUG:
        prio = ANDROID_LOG_DEBUG;
        break;
    case INFO:
        prio = ANDROID_LOG_INFO;
        break;
    case WARN:
        prio = ANDROID_LOG_WARN;
        break;
    case ERROR:
        prio = ANDROID_LOG_ERROR;
        break;
    default:
        prio = ANDROID_LOG_UNKNOWN;
        break;
    }

    if (!filename || !function || line <= 0) {
        __android_log_print(prio, android_tag, ">>> %s", message);
    } else {
        __android_log_print(prio, android_tag, "[%s@%s:%d]>>> %s", function, filename, line, message);
    }

    // maybe save the log string to file
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
