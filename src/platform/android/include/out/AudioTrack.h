#ifndef KT_AUDIO_TRACK_C_INCLUDE_H
#define KT_AUDIO_TRACK_C_INCLUDE_H

#include "J4A.h"


class AudioTrack {
public:
    AudioTrack(int samplerate, int channel, int format, int size, int sessionId);

    ~AudioTrack();

    void reCreate();



private:
    static void init_jni();
    static void uninit_jni();

    void New();


private:
    jobject _audiotrack{};

    int _samplerate;
    int _channel;
    int _format;
    int _size;
    int _sessionId;
};


#endif//KT_AUDIO_TRACK_C_INCLUDE_H
