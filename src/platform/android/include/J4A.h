#ifndef KT_J4A_INCLUDE_H
#define KT_J4A_INCLUDE_H

#include <jni.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum J4AFieldType {
    J4A_CLASS,
    J4A_FIELD,
    J4A_STATIC_FIELD,
    J4A_STATIC_FIELD_AS_INT,
    J4A_METHOD,
    J4A_STATIC_METHOD
} J4AFieldType;


typedef struct J4AField {
    const char *name;
    const char *method;
    const char *signature;
    int offset;
    int mandatory;
} J4AField;


/***************************************************************************************************
 * Exception Handle
 **************************************************************************************************/
bool J4A_ExceptionCheck__throwAny  (JNIEnv *env);
bool J4A_ExceptionCheck__catchAll  (JNIEnv *env);
int  J4A_ThrowExceptionOfClass     (JNIEnv *env, jclass clazz, const char *msg);
int  J4A_ThrowException            (JNIEnv *env, const char *class_sign, const char *msg);
int  J4A_ThrowIllegalStateException(JNIEnv *env, const char *msg);

/***************************************************************************************************
 * References
 **************************************************************************************************/
jobject J4A_NewGlobalRef__catchAll(JNIEnv *env, jobject obj);

void J4A_DeleteLocalRef    (JNIEnv *env, jobject  obj);
void J4A_DeleteLocalRef__p (JNIEnv *env, jobject *obj);
void J4A_DeleteGlobalRef   (JNIEnv *env, jobject  obj);
void J4A_DeleteGlobalRef__p(JNIEnv *env, jobject *obj);

void J4A_ReleaseStringUTFChars   (JNIEnv *env, jstring str, const char  *c_str);
void J4A_ReleaseStringUTFChars__p(JNIEnv *env, jstring str, const char **c_str);


/***************************************************************************************************
 * Class Load
 **************************************************************************************************/
int     J4A_LoadAll__catchAll               (JNIEnv *env);
jclass  J4A_FindClass__catchAll             (JNIEnv *env, const char *class_sign);
jobject J4A_FindClass__asGlobalRef__catchAll(JNIEnv *env, const char *class_sign);

jmethodID J4A_GetMethodID__catchAll      (JNIEnv *env, jclass clazz, const char *method_name,  const char *method_sign);
jmethodID J4A_GetStaticMethodID__catchAll(JNIEnv *env, jclass clazz, const char *method_name,  const char *method_sign);

jfieldID J4A_GetFieldID__catchAll      (JNIEnv *env, jclass clazz, const char *field_name, const char *method_sign);
jfieldID J4A_GetStaticFieldID__catchAll(JNIEnv *env, jclass clazz, const char *field_name, const char *method_sign);

/***************************************************************************************************
 * Misc Functions
 **************************************************************************************************/
jbyteArray J4A_NewByteArray__catchAll             (JNIEnv *env, jsize capacity);
jbyteArray J4A_NewByteArray__asGlobalRef__catchAll(JNIEnv *env, jsize capacity);


#ifdef __cplusplus
}
#endif

#endif//KT_J4A_INCLUDE_H
