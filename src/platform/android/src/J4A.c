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

/***************************************************************************************************
 * function like MPV jni
 ***************************************************************************************************/
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

static JavaVM *g_vm = NULL;
void J4A_SaveJvm(JavaVM *vm) {
    if(g_vm != vm)
        g_vm = vm;
}

static pthread_key_t   current_env;
static pthread_once_t  once = PTHREAD_ONCE_INIT;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void jni_detach_env(void *data) {
    if (g_vm) {
        (*g_vm)->DetachCurrentThread(g_vm);
    }
}

static void jni_create_pthread_key(void) {
    pthread_key_create(&current_env, jni_detach_env);
}

JNIEnv *J4A_GetEnv() {
    if(!g_vm) return NULL;

    int     ret = 0;
    JNIEnv *env = NULL;
    pthread_mutex_lock(&lock);
    pthread_once(&once, jni_create_pthread_key);
    if ((env = pthread_getspecific(current_env)) != NULL) {
        goto done;
    }
    ret = (*g_vm)->GetEnv(g_vm, (void **)&env, JNI_VERSION_1_6);
    switch(ret) {
        case JNI_EDETACHED:
            if ((*g_vm)->AttachCurrentThread(g_vm, &env, NULL) != 0) {
                LOG_ERROR(TAG, "Failed to attach the JNI environment to the current thread");
                env = NULL;
            } else {
                pthread_setspecific(current_env, env);
            }
            break;
        case JNI_OK:
            break;
        case JNI_EVERSION:
            LOG_ERROR(TAG, "The specified JNI version is not supported");
            break;
        default:
            LOG_ERROR(TAG, "Failed to get the JNI environment attached to this thread");
            break;
    }
done:
    pthread_mutex_unlock(&lock);
    return env;
}


char *J4A_JStringToUtfChar(JNIEnv *env, jstring string) {
    char       *ret       = NULL;
    const char *utf_chars = NULL;
    jboolean    copy      = 0;
    if(!string) return NULL;

    utf_chars = (const char *)(*env)->GetStringChars(env, string, &copy);
    if( (*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        LOG_ERROR(TAG, "String.GetStringChars throw an exception");
        return NULL;
    }

    ret = strdup(utf_chars);
    (*env)->ReleaseStringUTFChars(env, string, utf_chars);
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        LOG_ERROR(TAG, "String.releaseStringUTFChars() threw an exception");
        return NULL;
    }
    return ret;
}

jstring J4A_UtfCharToJString(JNIEnv *env, char *utf_chars) {
    jstring ret = (*env)->NewStringUTF(env, utf_chars);
    if ((*env)->ExceptionCheck(env)) {
        (*env)->ExceptionClear(env);
        LOG_ERROR(TAG, "NewStringUTF() threw an exception");
        return NULL;
    }
    return ret;
}

bool J4A_init_jfields(JNIEnv *env, void *jfields, const struct J4AField *jfields_mapping, int global) {
    bool ret = true;
    int  i   = 0;
    jclass last_clazz = NULL;
    for (i = 0; jfields_mapping[i].name; ++i) {
        int mandatory = jfields_mapping[i].mandatory;
        enum J4AFieldType type = jfields_mapping[i].type;
        if(type == J4A_CLASS) {
            jclass clazz;
            last_clazz = NULL;

            clazz      = (*env)->FindClass(env, jfields_mapping[i].name);
            if( (ret = J4A_ExceptionCheck__catchAll(env)) != true && mandatory ) {
                goto done;
            }

            last_clazz = *(jclass *)((uint8_t *)jfields + jfields_mapping[i].offset)
                         = global ? (*env)->NewGlobalRef(env, clazz) : clazz;
            if (global) {
                (*env)->DeleteLocalRef(env, clazz);
            }
        } else {
            if (!last_clazz) {
                ret = -1;
                break;
            }

            switch(type) {
                case J4A_FIELD: {
                        jfieldID field_id = (*env)->GetFieldID(env, last_clazz, jfields_mapping[i].method, jfields_mapping[i].signature);
                        if ((ret = J4A_ExceptionCheck__catchAll(env)) != true && mandatory) {
                            goto done;
                        }

                        *(jfieldID *)((uint8_t *)jfields + jfields_mapping[i].offset) = field_id;
                        break;
                    }
                case J4A_STATIC_FIELD_AS_INT:
                case J4A_STATIC_FIELD: {
                        jfieldID field_id = (*env)->GetStaticFieldID(env, last_clazz, jfields_mapping[i].method, jfields_mapping[i].signature);
                        if ((ret = J4A_ExceptionCheck__catchAll(env)) != true && mandatory) {
                            goto done;
                        }

                        if (type == J4A_STATIC_FIELD_AS_INT) {
                            if (field_id) {
                                jint value = (*env)->GetStaticIntField(env, last_clazz, field_id);
                                if ((ret = J4A_ExceptionCheck__catchAll(env)) != true && mandatory) {
                                    goto done;
                                }
                                *(jint *)((uint8_t *)jfields + jfields_mapping[i].offset) = value;
                            }
                        } else {
                            *(jfieldID *)((uint8_t *)jfields + jfields_mapping[i].offset) = field_id;
                        }
                        break;
                    }
                case J4A_METHOD: {
                        jmethodID method_id = (*env)->GetMethodID(env, last_clazz, jfields_mapping[i].method, jfields_mapping[i].signature);
                        if ((ret = J4A_ExceptionCheck__catchAll(env)) != true && mandatory) {
                            goto done;
                        }

                        *(jmethodID *)((uint8_t *)jfields + jfields_mapping[i].offset) = method_id;
                        break;
                    }
                case J4A_STATIC_METHOD: {
                        jmethodID method_id = (*env)->GetStaticMethodID(env, last_clazz, jfields_mapping[i].method, jfields_mapping[i].signature);
                        if ((ret = J4A_ExceptionCheck__catchAll(env)) != true && mandatory) {
                            goto done;
                        }

                        *(jmethodID *)((uint8_t *)jfields + jfields_mapping[i].offset) = method_id;
                        break;
                    }
                default:
                    LOG_ERROR(TAG, "Unknown JNI field type");
                    ret = false;
                    goto done;
            }
        }
    }

done:
    if(ret != true) {
        J4A_reset_jfields(env, jfields, jfields_mapping, global);
    }
    return ret;
}

bool J4A_reset_jfields(JNIEnv *env, void *jfields, const struct J4AField *jfields_mapping, int global) {
    int i = 0;
    for (i = 0; jfields_mapping[i].name; i++) {
        J4AFieldType type = jfields_mapping[i].type;
        switch(type) {
            case J4A_CLASS: {
                    jclass clazz = *(jclass *)((uint8_t *)jfields + jfields_mapping[i].offset);
                    if (!clazz)
                        continue;

                    if (global) {
                        (*env)->DeleteGlobalRef(env, clazz);
                    } else {
                        (*env)->DeleteLocalRef(env, clazz);
                    }

                    *(jclass *)((uint8_t *)jfields + jfields_mapping[i].offset) = NULL;
                    break;
                }
            case J4A_FIELD: {
                    *(jfieldID *)((uint8_t *)jfields + jfields_mapping[i].offset) = NULL;
                    break;
                }
            case J4A_STATIC_FIELD: {
                    *(jfieldID *)((uint8_t *)jfields + jfields_mapping[i].offset) = NULL;
                    break;
                }
            case J4A_STATIC_FIELD_AS_INT: {
                    *(jint *)((uint8_t *)jfields + jfields_mapping[i].offset) = 0;
                    break;
                }
            case J4A_METHOD: {
                    *(jmethodID *)((uint8_t *)jfields + jfields_mapping[i].offset) = NULL;
                    break;
                }
            case J4A_STATIC_METHOD: {
                    *(jmethodID *)((uint8_t *)jfields + jfields_mapping[i].offset) = NULL;
                    break;
                }
            default:
                LOG_ERROR(TAG, "Unknown JNI field type");
        }
    }

    return true;
}


#ifdef __cplusplus
}
#endif
