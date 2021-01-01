
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <jni.h>

#include "Logger.h"

static const char *TAG = "android-main";

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOG_INFO(TAG, "[JNI_OnLoad]");

    return JNI_VERSION_1_6; 
}

JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved) {
    LOG_INFO(TAG, "[JNI_OnUnload]");
}
