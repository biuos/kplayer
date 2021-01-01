#include "J4A.h"
#include "Logger.h"

#ifdef __cplusplus
extern "C" {
#endif

static const char *TAG = "J4A";

bool J4A_ExceptionCheck__throwAny(JNIEnv *env) {
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        return true;
    }

    return false;
}

bool J4A_ExceptionCheck__catchAll(JNIEnv *env) {
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionDescribe(env);
        (*env)->ExceptionClear(env);
        return true;
    }

    return false;
}

int J4A_ThrowExceptionOfClass(JNIEnv *env, jclass clazz, const char *msg) {
    if ((*env)->ThrowNew(env, clazz, msg) != JNI_OK) {
        LOG_ERROR(TAG, "J4A_ThrowExceptionOfClassP failed. msg:%s", msg);
    }

    return 0;
}

int J4A_ThrowException(JNIEnv *env, const char *class_sign, const char *msg) {
    int ret = -1;

    if (J4A_ExceptionCheck__catchAll(env)) {
        LOG_ERROR(TAG, "pending exception throwed.");
    }

    jclass exceptionClass = J4A_FindClass__catchAll(env, class_sign);
    if (exceptionClass == NULL) {
        LOG_ERROR(TAG, "J4A_FindClass__catchAll failed");
        ret = -1;
        goto fail;
    }

    ret = J4A_ThrowExceptionOfClass(env, exceptionClass, msg);
    if (ret) {
        LOG_ERROR(TAG, "J4A_ThrowExceptionOfClass failed");
        goto fail;
    }

    ret = 0;
fail:
    J4A_DeleteLocalRef__p(env, &exceptionClass);
    return ret;
}

int J4A_ThrowIllegalStateException(JNIEnv *env, const char *msg) {
    return J4A_ThrowException(env, "java/lang/IllegalStateException", msg);
}


jclass J4A_NewGlobalRef__catchAll(JNIEnv *env, jobject obj) {
    jclass obj_global = (*env)->NewGlobalRef(env, obj);
    if (J4A_ExceptionCheck__catchAll(env) || !(obj_global)) {
        LOG_ERROR(TAG, "J4A_NewGlobalRef__catchAll failed");
        goto fail;
    }

fail:
    return obj_global;
}

void J4A_DeleteLocalRef(JNIEnv *env, jobject obj) {
    if (!obj) return;
    (*env)->DeleteLocalRef(env, obj);
}

void J4A_DeleteLocalRef__p(JNIEnv *env, jobject *obj) {
    if (!obj) return;
    J4A_DeleteLocalRef(env, *obj);
    *obj = NULL;
}

void J4A_DeleteGlobalRef(JNIEnv *env, jobject obj) {
    if (!obj) return;
    (*env)->DeleteGlobalRef(env, obj);
}

void J4A_DeleteGlobalRef__p(JNIEnv *env, jobject *obj) {
    if (!obj) return;
    J4A_DeleteGlobalRef(env, *obj);
    *obj = NULL;
}

void J4A_ReleaseStringUTFChars(JNIEnv *env, jstring str, const char *c_str) {
    if (!str || !c_str) return;
    (*env)->ReleaseStringUTFChars(env, str, c_str);
}

void J4A_ReleaseStringUTFChars__p(JNIEnv *env, jstring str, const char **c_str) {
    if (!str || !c_str) return;
    J4A_ReleaseStringUTFChars(env, str, *c_str);
    *c_str = NULL;
}

jclass J4A_FindClass__catchAll(JNIEnv *env, const char *class_sign) {
    jclass clazz = (*env)->FindClass(env, class_sign);
    if (J4A_ExceptionCheck__catchAll(env) || !(clazz)) {
        LOG_ERROR(TAG, "J4A_FindClass__catchAll: failed");
        clazz = NULL;
        goto fail;
    }
fail:
    return clazz;
}

jclass J4A_FindClass__asGlobalRef__catchAll(JNIEnv *env, const char *class_sign) {
    jclass clazz_global = NULL;
    jclass clazz = J4A_FindClass__catchAll(env, class_sign);
    if (!clazz) {
        LOG_ERROR(TAG, "J4A_FindClass__catchAll: failed. %s", class_sign);
        goto fail;
    }

    clazz_global = J4A_NewGlobalRef__catchAll(env, clazz);
    if (!clazz_global) {
        LOG_ERROR(TAG, "J4A_NewGlobalRef__catchAll: failed. %s", class_sign);
        goto fail;
    }

fail:
    J4A_DeleteLocalRef__p(env, &clazz);
    return clazz_global;
}

jmethodID J4A_GetMethodID__catchAll(JNIEnv *env, jclass clazz, const char *method_name,
                                    const char *method_sign) {
    jmethodID method_id = (*env)->GetMethodID(env, clazz, method_name, method_sign);
    if (J4A_ExceptionCheck__catchAll(env) || !method_id) {
        LOG_ERROR(TAG, "J4A_GetMethodID__catchAll: failed. %s:%s", method_name, method_sign);
        method_id = NULL;
        goto fail;
    }
fail:
    return method_id;
}

jmethodID J4A_GetStaticMethodID__catchAll(JNIEnv *env, jclass clazz, const char *method_name,
                                          const char *method_sign) {
    jmethodID method_id = (*env)->GetStaticMethodID(env, clazz, method_name, method_sign);
    if (J4A_ExceptionCheck__catchAll(env) || !method_id) {
        LOG_ERROR(TAG, "J4A_GetStaticMethodID__catchAll: failed. %s:%s", method_name, method_sign);
        method_id = NULL;
        goto fail;
    }
fail:
    return method_id;
}

jfieldID J4A_GetFieldID__catchAll(JNIEnv *env, jclass clazz, const char *field_name,
                                  const char *field_sign) {
    jfieldID field_id = (*env)->GetFieldID(env, clazz, field_name, field_sign);
    if (J4A_ExceptionCheck__catchAll(env) || !field_id) {
        LOG_ERROR(TAG, "J4A_GetFieldID__catchAll: failed. %s:%s", field_name, field_sign);
        field_id = NULL;
        goto fail;
    }

fail:
    return field_id;
}

jfieldID J4A_GetStaticFieldID__catchAll(JNIEnv *env, jclass clazz, const char *field_name,
                                        const char *field_sign) {
    jfieldID field_id = (*env)->GetStaticFieldID(env, clazz, field_name, field_sign);
    if (J4A_ExceptionCheck__catchAll(env) || !field_id) {
        LOG_ERROR(TAG, "J4A_GetStaticFieldID__catchAll: failed. %s:%s", field_name, field_sign);
        field_id = NULL;
        goto fail;
    }

fail:
    return field_id;
}

jbyteArray J4A_NewByteArray__catchAll(JNIEnv *env, jsize capacity) {
    jbyteArray local = (*env)->NewByteArray(env, capacity);
    if (J4A_ExceptionCheck__catchAll(env) || !local)
        return NULL;

    return local;
}

jbyteArray J4A_NewByteArray__asGlobalRef__catchAll(JNIEnv *env, jsize capacity) {
    jbyteArray local = (*env)->NewByteArray(env, capacity);
    if (J4A_ExceptionCheck__catchAll(env) || !local)
        return NULL;

    jbyteArray global = (*env)->NewGlobalRef(env, local);
    J4A_DeleteLocalRef__p(env, &local);
    return global;
}

#ifdef __cplusplus
}
#endif
