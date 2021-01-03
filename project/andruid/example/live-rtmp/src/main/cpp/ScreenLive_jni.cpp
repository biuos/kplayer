#include <jni.h>
#include <string>

#include <android/log.h>

#include "librtmp/rtmp.h"


static const char *tag = "ScreenLive-JNI";

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, tag, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, tag, __VA_ARGS__)

struct Live {
    int16_t sps_len;
    int16_t pps_len;
    int8_t *sps;
    int8_t *pps;
    RTMP *rtmp;
};

static Live *gLive = nullptr;

static void prepare_video(int8_t *data, int len, Live *live) {
    for (int i = 0; i < len; ++i) {
        // 0x00  0x00  0x00  0x01
        if (i + 4 < len) {
            if (data[i + 0] == 0x00 &&
                data[i + 1] == 0x00 &&
                data[i + 2] == 0x00 &&
                data[i + 3] == 0x01) {
                if (data[i + 4] == 0x68) {
                    //0x00 0x00 0x00 0x01 7 sps 0x00 0x00 0x00 0x01 8 pps
                    //将sps pps分开
                    //找到pps
                    live->sps_len = i - 4;
                    live->sps = static_cast<int8_t *>(malloc(live->sps_len));
                    memcpy(live->sps, data + 4, live->sps_len);
                    live->pps_len = len - (4 + live->sps_len) - 4;
                    live->pps = static_cast<int8_t *>(malloc(live->pps_len));
                    memcpy(live->pps, data + 4 + live->sps_len + 4, live->pps_len);
                    LOGI("sps:%d pps:%d", live->sps_len, live->pps_len);
                    break;
                }
            }
        }
    }
}

static RTMPPacket *create_video_package(Live *live) {
    int body_size = 13 + live->sps_len + 3 + live->pps_len;
    RTMPPacket *packet = (RTMPPacket *) malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, body_size);
    int i = 0;
    //AVC sequence header 与IDR一样
    packet->m_body[i++] = 0x17;
    //AVC sequence header 设置为0x00
    packet->m_body[i++] = 0x00;
    //CompositionTime
    packet->m_body[i++] = 0x00;
    packet->m_body[i++] = 0x00;
    packet->m_body[i++] = 0x00;
    //AVC sequence header
    packet->m_body[i++] = 0x01;   //configurationVersion 版本号 1
    packet->m_body[i++] = live->sps[1]; //profile 如baseline、main、 high
    packet->m_body[i++] = live->sps[2]; //profile_compatibility 兼容性
    packet->m_body[i++] = live->sps[3]; //profile level
    packet->m_body[i++] = 0xFF; // reserved（111111） + lengthSizeMinusOne（2位 nal 长度） 总是0xff
    //sps
    packet->m_body[i++] = 0xE1; //reserved（111） + lengthSizeMinusOne（5位 sps 个数） 总是0xe1
    //sps length 2字节
    packet->m_body[i++] = (live->sps_len >> 8) & 0xff; //第0个字节
    packet->m_body[i++] = live->sps_len & 0xff;        //第1个字节
    memcpy(&packet->m_body[i], live->sps, live->sps_len);
    i += live->sps_len;

    /*pps*/
    packet->m_body[i++] = 0x01; //pps number
    //pps length
    packet->m_body[i++] = (live->pps_len >> 8) & 0xff;
    packet->m_body[i++] = live->pps_len & 0xff;
    memcpy(&packet->m_body[i], live->pps, live->pps_len);

    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize = body_size;
    packet->m_nChannel = 0x04;
    packet->m_nTimeStamp = 0;
    packet->m_hasAbsTimestamp = 0;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nInfoField2 = live->rtmp->m_stream_id;
    return packet;
}

static RTMPPacket *create_video_package(int8_t *buf, int len, const long tms, Live *live) {
    buf += 4;
    len -= 4;
    int body_size = len + 9;
    RTMPPacket *packet = (RTMPPacket *) malloc(sizeof(RTMPPacket));
    RTMPPacket_Alloc(packet, len + 9);

    packet->m_body[0] = 0x27;
    if (buf[0] == 0x65) { //关键帧
        packet->m_body[0] = 0x17;
        LOGI("发送关键帧 data");
    }
    packet->m_body[1] = 0x01;
    packet->m_body[2] = 0x00;
    packet->m_body[3] = 0x00;
    packet->m_body[4] = 0x00;

    //长度
    packet->m_body[5] = (len >> 24) & 0xff;
    packet->m_body[6] = (len >> 16) & 0xff;
    packet->m_body[7] = (len >> 8) & 0xff;
    packet->m_body[8] = (len) & 0xff;

    //数据
    memcpy(&packet->m_body[9], buf, len);


    packet->m_packetType = RTMP_PACKET_TYPE_VIDEO;
    packet->m_nBodySize = body_size;
    packet->m_nChannel = 0x04;
    packet->m_nTimeStamp = tms;
    packet->m_hasAbsTimestamp = 0;
    packet->m_headerType = RTMP_PACKET_SIZE_LARGE;
    packet->m_nInfoField2 = live->rtmp->m_stream_id;
    return packet;
}

static int send_packet(Live *live, RTMPPacket *pkt) {
    int ret = RTMP_SendPacket(live->rtmp, pkt, 1);
    RTMPPacket_Free(pkt);
    free(pkt);
    return ret;
}

static int send_video(int8_t *buffer, int len, long tms) {
    LOGI("send_video: len=%d tms=%ld", len, tms);
    int ret = 0;
    if (!gLive) {
        LOGE("allocate the Live object first");
        return -1;
    }

    // sps pps
    if (buffer[4] == 0x67) {
        // sps or pps
        if (!gLive->pps || !gLive->sps) {
            prepare_video(buffer, len, gLive);
        }
    } else {
        // key frame
        if (buffer[4] == 0x65) {
            RTMPPacket *packet = create_video_package(gLive);
            ret = send_packet(gLive, packet);
            if (ret != 0) {
                LOGI("1-send_video failed");
            }
        }

        RTMPPacket *packet = create_video_package(buffer, len, tms, gLive);
        ret = send_packet(gLive, packet);
        if (ret != 0) {
            LOGI("2-send_video failed");
        }
    }
    return ret;
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_dai_android_live_example_ScreenLive_nativeConnect(JNIEnv *env, jobject thiz, jstring url) {
    LOGI("ScreenLive_nativeConnect");

    int ret = 0;
    const char *c_url = env->GetStringUTFChars(url, nullptr);
    LOGI("nativeConnect: %s", c_url);

    do {
        gLive = new Live;

        gLive->rtmp = RTMP_Alloc();
        if (!gLive->rtmp) {
            LOGE("RTMP_Alloc: failed");
            break;
        }

        RTMP_Init(gLive->rtmp);
        gLive->rtmp->Link.timeout = 15;

        LOGI("1- RTMP_SetupURL");
        if (!(ret = RTMP_SetupURL(gLive->rtmp, (char *) c_url))) {
            LOGE("1- RTMP_SetupURL: failed");
            break;
        }
        RTMP_EnableWrite(gLive->rtmp);


        LOGI("2- RTMP_Connect");
        if (!(ret = RTMP_Connect(gLive->rtmp, nullptr))) {
            LOGI("2- RTMP_Connect: failed");
            break;
        }


        LOGI("3- RTMP_ConnectStream");
        if (!(ret = RTMP_Connect(gLive->rtmp, nullptr))) {
            LOGI("3- RTMP_ConnectStream: failed");
            break;
        }
    } while (false);

    if (!ret) {
        LOGE("create Live object failed");
        delete gLive;
        gLive = nullptr;
    }

    env->ReleaseStringChars(url, reinterpret_cast<const jchar *>(c_url));

    return ret;
}

extern "C"
JNIEXPORT void JNICALL
Java_dai_android_live_example_ScreenLive_nativeSendData(
        JNIEnv *env, jobject thiz, jbyteArray data_, jint len, jint type, jlong tms) {

    int ret = 0;
    jbyte *data = env->GetByteArrayElements(data_, nullptr);
    send_video(data, len, tms);
    env->ReleaseByteArrayElements(data_, data, 0);
}

extern "C"
JNIEXPORT void JNICALL
Java_dai_android_live_example_ScreenLive_nativeRelease(JNIEnv *env, jobject thiz) {
    LOGI("ScreenLive_nativeRelease");

    if (gLive) {
        delete gLive;
        gLive = nullptr;
    }
}