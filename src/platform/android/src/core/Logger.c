
#include "Logger.h"

#include <stdio.h>
#include <stdarg.h>

#include <android/log.h>


#ifdef __cplusplus
extern "C" {
#endif


static const char *default_tag  = "Logger";
static LogLevel g_min_log_level = LOG_ERROR;


void set_min_logger_level(LogLevel min_level) {
    g_min_log_level = min_level;
}

void logger_message(LogLevel level,
                    const char *tag,
                    const char *filename, const char *func, int line,
                    const char *fmt, ...) {
    if (level < g_min_log_level) return;

    const char *android_tag = tag;
    if (!android_tag) android_tag = default_tag;
    int prio = ANDROID_LOG_UNKNOWN;
    switch (level) {
        case LOG_VERBOSE:
            prio = ANDROID_LOG_VERBOSE;
            break;
        case LOG_DEBUG:
            prio = ANDROID_LOG_DEBUG;
            break;
        case LOG_INFO:
            prio = ANDROID_LOG_INFO;
            break;
        case LOG_WARN:
            prio = ANDROID_LOG_WARN;
            break;
        case LOG_ERROR:
            prio = ANDROID_LOG_ERROR;
            break;
        default:
            prio = ANDROID_LOG_UNKNOWN;
            break;
    }

    static const size_t SIZE = 4096; // 4K
    char buffer[SIZE] = {0x00};
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, SIZE, fmt, args);
    va_end(args);

    if (!filename || !func || line <= 0) {
        __android_log_print(prio, android_tag, ">>> %s", buffer);
    } else {
        __android_log_print(prio, android_tag, "[%s@%s:%d]>>> %s", func, filename, line, buffer);
    }

    // maybe save the log string to file
    // maybe use jni call save log file
}


#ifdef __cplusplus
}
#endif