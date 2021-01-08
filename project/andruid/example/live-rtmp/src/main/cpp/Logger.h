
#ifndef LIVE_LOGGER_INCLUDE_H
#define LIVE_LOGGER_INCLUDE_H

#include <android/log.h>

#define ANDROID_LOG_TAG "NDK4Live"

#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, ANDROID_LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO,  ANDROID_LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, ANDROID_LOG_TAG, __VA_ARGS__)

#endif //LIVE_LOGGER_INCLUDE_H

