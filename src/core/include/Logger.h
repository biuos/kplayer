#ifndef KT_LOGGER_INCLUDE_H
#define KT_LOGGER_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum LogLevel {
    LOG_VERBOSE = 2,
    LOG_DEBUG   = 3,
    LOG_INFO    = 4,
    LOG_WARN    = 5,
    LOG_ERROR   = 6
} LogLevel;


extern void set_min_logger_level( LogLevel min_level );
extern void logger_message( LogLevel level,
                                const char *tag,
                                const char *filename, const char *func, int line,
                                const char *fmt, ...);

#define LOG_VERBOSE(tag, fmt, ...)                                                                  \
    do {                                                                                            \
        logger_message(LOG_VERBOSE, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);\
    } while(0)

#define LOG_DEBUG(tag, fmt, ...)                                                                   \
    do {                                                                                           \
        logger_message(LOG_DEBUG, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    } while(0)

#define LOG_INFO(tag, fmt, ...)                                                                    \
    do {                                                                                           \
        logger_message(LOG_INFO, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);  \
    } while(0)

#define LOG_WARN(tag, fmt, ...)                                                                    \
    do {                                                                                           \
        logger_message(LOG_WARN, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);  \
    } while(0)

#define LOG_ERROR(tag, fmt, ...)                                                                   \
    do {                                                                                           \
        logger_message(LOG_ERROR, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif//KT_LOGGER_INCLUDE_H
