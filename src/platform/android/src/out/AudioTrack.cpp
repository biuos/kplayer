#include <jni.h>

#include "out/AudioTrack.h"
#include "Logger.h"


#include <memory>

struct JNIByteBuffer {
    jclass    clazz;
    jmethodID clear;
    struct J4AField mapping[3];
} _ByteBuffer = {.mapping = {
#define OFFSET(member) offsetof(struct JNIByteBuffer, member)
        {"java/nio/ByteBuffer", NULL,    NULL,                  J4A_CLASS,  OFFSET(clazz), 1},
        {"java/nio/ByteBuffer", "clear", "()Ljava/nio/Buffer;", J4A_METHOD, OFFSET(clear), 1},
        {0},
#undef OFFSET
    }
};

struct JNIAudioTrack {
    jclass clazz;
    jmethodID ctor;
    jmethodID release;
    jmethodID getState;
    jmethodID getPlayState;
    jmethodID play;
    jmethodID stop;
    jmethodID flush;
    jmethodID pause;
    jmethodID write;
    jmethodID writeFloat;
    jmethodID writeV23;
    jmethodID writeShortV23;
    jmethodID writeBufferV21;
    jmethodID getPlaybackHeadPosition;
    jmethodID getTimestamp;
    jmethodID getLatency;
    jmethodID getMinBufferSize;
    jmethodID getNativeOutputSampleRate;
    jint STATE_INITIALIZED;
    jint PLAYSTATE_STOPPED;
    jint PLAYSTATE_PAUSED;
    jint PLAYSTATE_PLAYING;
    jint MODE_STREAM;
    jint ERROR;
    jint ERROR_BAD_VALUE;
    jint ERROR_INVALID_OPERATION;
    jint WRITE_BLOCKING;
    jint WRITE_NON_BLOCKING;
    struct J4AField mapping[30];
} _AudioTrack = {.mapping = {
#define OFFSET(member) offsetof(struct JNIAudioTrack, member)
        {"android/media/AudioTrack", NULL,                        NULL,                                J4A_CLASS,               OFFSET(clazz),                     1},
        {"android/media/AudioTrack", "<init>",                    "(IIIIIII)V",                        J4A_METHOD,              OFFSET(ctor),                      1},
        {"android/media/AudioTrack", "release",                   "()V",                               J4A_METHOD,              OFFSET(release),                   1},
        {"android/media/AudioTrack", "getState",                  "()I",                               J4A_METHOD,              OFFSET(getState),                  1},
        {"android/media/AudioTrack", "getPlayState",              "()I",                               J4A_METHOD,              OFFSET(getPlayState),              1},
        {"android/media/AudioTrack", "play",                      "()V",                               J4A_METHOD,              OFFSET(play),                      1},
        {"android/media/AudioTrack", "stop",                      "()V",                               J4A_METHOD,              OFFSET(stop),                      1},
        {"android/media/AudioTrack", "flush",                     "()V",                               J4A_METHOD,              OFFSET(flush),                     1},
        {"android/media/AudioTrack", "pause",                     "()V",                               J4A_METHOD,              OFFSET(pause),                     1},
        {"android/media/AudioTrack", "write",                     "([BII)I",                           J4A_METHOD,              OFFSET(write),                     1},
        {"android/media/AudioTrack", "write",                     "([FIII)I",                          J4A_METHOD,              OFFSET(writeFloat),                1},
        {"android/media/AudioTrack", "write",                     "([BIII)I",                          J4A_METHOD,              OFFSET(writeV23),                  0},
        {"android/media/AudioTrack", "write",                     "([SIII)I",                          J4A_METHOD,              OFFSET(writeShortV23),             0},
        {"android/media/AudioTrack", "write",                     "(Ljava/nio/ByteBuffer;II)I",        J4A_METHOD,              OFFSET(writeBufferV21),            1},
        {"android/media/AudioTrack", "getTimestamp",              "(Landroid/media/AudioTimestamp;)Z", J4A_METHOD,              OFFSET(getTimestamp),              1},
        {"android/media/AudioTrack", "getPlaybackHeadPosition",   "()I",                               J4A_METHOD,              OFFSET(getPlaybackHeadPosition),   1},
        {"android/media/AudioTrack", "getLatency",                "()I",                               J4A_METHOD,              OFFSET(getLatency),                1},
        {"android/media/AudioTrack", "getMinBufferSize",          "(III)I",                            J4A_STATIC_METHOD,       OFFSET(getMinBufferSize),          1},
        {"android/media/AudioTrack", "getNativeOutputSampleRate", "(I)I",                              J4A_STATIC_METHOD,       OFFSET(getNativeOutputSampleRate), 1},
        {"android/media/AudioTrack", "WRITE_BLOCKING",            "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(WRITE_BLOCKING),            0},
        {"android/media/AudioTrack", "WRITE_NON_BLOCKING",        "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(WRITE_NON_BLOCKING),        0},
        {"android/media/AudioTrack", "STATE_INITIALIZED",         "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(STATE_INITIALIZED),         1},
        {"android/media/AudioTrack", "PLAYSTATE_STOPPED",         "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(PLAYSTATE_STOPPED),         1},
        {"android/media/AudioTrack", "PLAYSTATE_PAUSED",          "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(PLAYSTATE_PAUSED),          1},
        {"android/media/AudioTrack", "PLAYSTATE_PLAYING",         "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(PLAYSTATE_PLAYING),         1},
        {"android/media/AudioTrack", "MODE_STREAM",               "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(MODE_STREAM),               1},
        {"android/media/AudioTrack", "ERROR",                     "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(ERROR),                     1},
        {"android/media/AudioTrack", "ERROR_BAD_VALUE",           "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(ERROR_BAD_VALUE),           1},
        {"android/media/AudioTrack", "ERROR_INVALID_OPERATION",   "I",                                 J4A_STATIC_FIELD_AS_INT, OFFSET(ERROR_INVALID_OPERATION),   1},
        {0}
#undef OFFSET
    }
};

struct JNIAudioFormat {
    jclass clazz;
    jint ENCODING_PCM_8BIT;
    jint ENCODING_PCM_16BIT;
    jint ENCODING_PCM_FLOAT;
    jint ENCODING_IEC61937;
    jint ENCODING_AC3;
    jint CHANNEL_OUT_MONO;
    jint CHANNEL_OUT_STEREO;
    jint CHANNEL_OUT_FRONT_LEFT;
    jint CHANNEL_OUT_FRONT_RIGHT;
    jint CHANNEL_OUT_BACK_LEFT;
    jint CHANNEL_OUT_BACK_RIGHT;
    jint CHANNEL_OUT_FRONT_CENTER;
    jint CHANNEL_OUT_LOW_FREQUENCY;
    jint CHANNEL_OUT_BACK_CENTER;
    jint CHANNEL_OUT_5POINT1;
    jint CHANNEL_OUT_SIDE_LEFT;
    jint CHANNEL_OUT_SIDE_RIGHT;
    struct J4AField mapping[19];
} _AudioFormat = {.mapping = {
#define OFFSET(member) offsetof(struct JNIAudioFormat, member)
        {"android/media/AudioFormat", NULL,                        NULL, J4A_CLASS,               OFFSET(clazz),                     1},
        {"android/media/AudioFormat", "ENCODING_PCM_8BIT",         "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(ENCODING_PCM_8BIT),         1},
        {"android/media/AudioFormat", "ENCODING_PCM_16BIT",        "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(ENCODING_PCM_16BIT),        1},
        {"android/media/AudioFormat", "ENCODING_PCM_FLOAT",        "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(ENCODING_PCM_FLOAT),        1},
        {"android/media/AudioFormat", "ENCODING_AC3",              "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(ENCODING_AC3),              0},
        {"android/media/AudioFormat", "ENCODING_IEC61937",         "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(ENCODING_IEC61937),         0},
        {"android/media/AudioFormat", "CHANNEL_OUT_MONO",          "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_MONO),          1},
        {"android/media/AudioFormat", "CHANNEL_OUT_STEREO",        "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_STEREO),        1},
        {"android/media/AudioFormat", "CHANNEL_OUT_5POINT1",       "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_5POINT1),       1},
        {"android/media/AudioFormat", "CHANNEL_OUT_FRONT_LEFT",    "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_FRONT_LEFT),    1},
        {"android/media/AudioFormat", "CHANNEL_OUT_FRONT_RIGHT",   "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_FRONT_RIGHT),   1},
        {"android/media/AudioFormat", "CHANNEL_OUT_FRONT_CENTER",  "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_FRONT_CENTER),  1},
        {"android/media/AudioFormat", "CHANNEL_OUT_LOW_FREQUENCY", "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_LOW_FREQUENCY), 1},
        {"android/media/AudioFormat", "CHANNEL_OUT_BACK_LEFT",     "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_BACK_LEFT),     1},
        {"android/media/AudioFormat", "CHANNEL_OUT_BACK_RIGHT",    "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_BACK_RIGHT),    1},
        {"android/media/AudioFormat", "CHANNEL_OUT_BACK_CENTER",   "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_BACK_CENTER),   1},
        {"android/media/AudioFormat", "CHANNEL_OUT_SIDE_LEFT",     "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_SIDE_LEFT),     0},
        {"android/media/AudioFormat", "CHANNEL_OUT_SIDE_RIGHT",    "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(CHANNEL_OUT_SIDE_RIGHT),    0},
        {0}
#undef OFFSET
    }
};


struct JNIAudioManager {
    jclass clazz;
    jint ERROR_DEAD_OBJECT;
    jint STREAM_MUSIC;
    struct J4AField mapping[4];
} _AudioManager = {.mapping = {
#define OFFSET(member) offsetof(struct JNIAudioManager, member)
        {"android/media/AudioManager", NULL,                NULL, J4A_CLASS,               OFFSET(clazz),             1},
        {"android/media/AudioManager", "STREAM_MUSIC",      "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(STREAM_MUSIC),      1},
        {"android/media/AudioManager", "ERROR_DEAD_OBJECT", "I",  J4A_STATIC_FIELD_AS_INT, OFFSET(ERROR_DEAD_OBJECT), 0},
        {0}
#undef OFFSET
    }
};


struct JNIAudioTimestamp {
    jclass clazz;
    jmethodID ctor;
    jfieldID framePosition;
    jfieldID nanoTime;
    struct J4AField mapping[5];
} _AudioTimestamp = {.mapping = {
#define OFFSET(member) offsetof(struct JNIAudioTimestamp, member)
        {"android/media/AudioTimestamp", NULL,            NULL,  J4A_CLASS,  OFFSET(clazz),         1},
        {"android/media/AudioTimestamp", "<init>",        "()V", J4A_METHOD, OFFSET(ctor),          1},
        {"android/media/AudioTimestamp", "framePosition", "J",   J4A_FIELD,  OFFSET(framePosition), 1},
        {"android/media/AudioTimestamp", "nanoTime",      "J",   J4A_FIELD,  OFFSET(nanoTime),      1},
        {0}
#undef OFFSET
    }
};


static const char *TAG = "AudioTrackJni";
static std::atomic_bool inited{false};



void AudioTrack::init_jni() {
    if(inited == true) {
        return;
    }

    JNIEnv *env = J4A_GetEnv();
    if (!J4A_init_jfields(env, &_AudioTrack,     _AudioTrack.mapping, 1) ||
        !J4A_init_jfields(env, &_ByteBuffer,     _ByteBuffer.mapping, 1) ||
        !J4A_init_jfields(env, &_AudioTimestamp, _AudioTimestamp.mapping, 1) ||
        !J4A_init_jfields(env, &_AudioManager,   _AudioManager.mapping, 1) ||
        !J4A_init_jfields(env, &_AudioFormat,    _AudioFormat.mapping, 1)) {
        uninit_jni();
        return;
    }
    inited = true;
}

void AudioTrack::uninit_jni() {
    JNIEnv *env = J4A_GetEnv();
    J4A_reset_jfields(env, &_AudioTrack,     _AudioTrack.mapping,     1);
    J4A_reset_jfields(env, &_AudioTimestamp, _AudioTimestamp.mapping, 1);
    J4A_reset_jfields(env, &_AudioManager,   _AudioManager.mapping,   1);
    J4A_reset_jfields(env, &_AudioFormat,    _AudioFormat.mapping,    1);
    J4A_reset_jfields(env, &_ByteBuffer,     _ByteBuffer.mapping,     1);
    inited = false;
}

AudioTrack::AudioTrack(int samplerate, int channel, int format, int size, int sessionId):
    _samplerate(samplerate),
    _channel(channel),
    _format(format),
    _size(size),
    _sessionId(sessionId) {
    init_jni();
    New();
}


void AudioTrack::New() {
    JNIEnv *env        = J4A_GetEnv();
    jobject audiotrack = J4A_NEW( _AudioTrack.clazz,
                                  _AudioTrack.ctor, _AudioManager.STREAM_MUSIC,
                                  _samplerate, _channel, _format, _size, _AudioTrack.MODE_STREAM, _sessionId
                                );
    if(!audiotrack) {
        LOG_ERROR(TAG, "JNI create AudioTrack failed");
        return;
    }

    if( J4A_CALL_INT(audiotrack, _AudioTrack.getState) != _AudioTrack.STATE_INITIALIZED) {
        J4A_CALL_VOID(audiotrack, _AudioTrack.release);
        env->DeleteLocalRef(audiotrack);
        return;
    }
    _audiotrack = env->NewGlobalRef(audiotrack);
    env->DeleteLocalRef(audiotrack);
}

void AudioTrack::reCreate() {
    if(_audiotrack) {
        JNIEnv *env = J4A_GetEnv();
        J4A_CALL_VOID(_audiotrack, _AudioTrack.release);
        env->DeleteGlobalRef(_audiotrack);
        _audiotrack = nullptr;
    }
    New();
}
