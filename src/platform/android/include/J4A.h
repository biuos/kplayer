#ifndef KT_J4A_INCLUDE_H
#define KT_J4A_INCLUDE_H

#include <jni.h>

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


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


/***************************************************************************************************
 * function like MPV jni
 **************************************************************************************************/
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
    J4AFieldType type;
    int          offset;
    int          mandatory;
} J4AField;

void    J4A_SaveJvm(JavaVM *vm);
JNIEnv *J4A_GetEnv();

char   *J4A_JStringToUtfChar(JNIEnv *env, jstring string);
jstring J4A_UtfCharToJString(JNIEnv *env, char *utf_chars);

bool J4A_init_jfields (JNIEnv *env, void *jfields, const struct J4AField *jfields_mapping, int global);
bool J4A_reset_jfields(JNIEnv *env, void *jfields, const struct J4AField *jfields_mapping, int global);


#ifdef __cplusplus
#   define J4A_DO( what, obj,   method, ...)    env->what(obj, method, ##__VA_ARGS__)
#else
#   define J4A_DO( what, obj,   method, ...) (*env)->what(env, obj, method, ##__VA_ARGS__)
#endif
#define        J4A_CALL_INT(obj,   method, ...) J4A_DO( CallIntMethod,       obj,   method, ##__VA_ARGS__)
#define J4A_CALL_STATIC_INT(clazz, method, ...) J4A_DO( CallStaticIntMethod, clazz, method, ##__VA_ARGS__)
#define       J4A_CALL_BOOL(obj,   method, ...) J4A_DO( CallBooleanMethod,   obj,   method, ##__VA_ARGS__)
#define       J4A_CALL_VOID(obj,   method, ...) J4A_DO( CallVoidMethod,      obj,   method, ##__VA_ARGS__)
#define     J4A_CALL_OBJECT(obj,   method, ...) J4A_DO( CallObjectMethod,    obj,   method, ##__VA_ARGS__)
#define             J4A_NEW(clazz, method, ...) J4A_DO( NewObject,           clazz, method, ##__VA_ARGS__)
#define         J4A_GET_INT(obj,         field) J4A_DO( GetIntField,         obj, field)
#define        J4A_GET_LONG(obj,         field) J4A_DO( GetLongField,        obj, field)
#define        J4A_GET_BOOL(obj,         field) J4A_DO( GetBoolField,        obj, field)


#ifdef __cplusplus
}
#endif

#endif//KT_J4A_INCLUDE_H
