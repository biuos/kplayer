#ifndef KT_LOGGER_INCLUDE_H
#define KT_LOGGER_INCLUDE_H

#include "Define.h"

#include <mutex>

NS_KT_START

//class Logger;

class Logger {
public:
    enum LogLevel {
        VERBOSE = 2,
        DEBUG   = 3,
        INFO    = 4,
        WARN    = 5,
        ERROR   = 6
    };

    ~Logger() = default;

    Logger(const Logger&)            = delete;
    Logger(const Logger&&)           = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default;

    void logger(int level,
                const char *tag,
                const char *filename, const char *function, int line,
                const char *message);

    static std::mutex _s_mutex;
    static Logger    *_s_instance;

public:
    static Logger& getLogger();

    static void init(void *priv);

    void verbose(const char *tag,
                 const char *filename, const char *function, int line,
                 const char *message) {
        logger(VERBOSE, tag, filename, function, line, message);
    }

    void debug(const char *tag,
               const char *filename, const char *function, int line,
               const char *message) {
        logger(DEBUG, tag, filename, function, line, message);
    }

    void info(const char *tag,
              const char *filename, const char *function, int line,
              const char *message) {
        logger(INFO, tag, filename, function, line, message);
    }

    void warn(const char *tag,
              const char *filename, const char *function, int line,
              const char *message) {
        logger(WARN, tag, filename, function, line, message);
    }

    void error(const char *tag,
               const char *filename, const char *function, int line,
               const char *message) {
        logger(ERROR, tag, filename, function, line, message);
    }


    void message(int level,
                 const char *tag,
                 const char *filename, const char *function, int line,
                 const char *fmt, ...);
};

NS_KT_END


#define LOG_VERBOSE(tag, fmt, ...) \
    do { \
        kt::Logger::getLogger().message(kt::Logger::VERBOSE, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    } while(0)

#define LOG_DEBUG(tag, fmt, ...) \
    do { \
        kt::Logger::getLogger().message(kt::Logger::DEBUG, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    } while(0)

#define LOG_INFO(tag, fmt, ...) \
    do { \
        kt::Logger::getLogger().message(kt::Logger::INFO, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    } while(0)

#define LOG_WARN(tag, fmt, ...) \
    do { \
        kt::Logger::getLogger().message(kt::Logger::WARN, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    } while(0)

#define LOG_ERROR(tag, fmt, ...) \
    do { \
        kt::Logger::getLogger().message(kt::Logger::ERROR, tag, __FILE_NAME__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    } while(0)

#endif//KT_LOGGER_INCLUDE_H
