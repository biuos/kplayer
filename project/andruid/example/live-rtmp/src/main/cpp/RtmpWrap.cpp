
#include "RtmpWrap.h"
#include "Logger.h"

#include <memory>
#include <memory.h>

namespace live {

static const size_t RTMP_HEAD_SIZE = (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE);

RtmpWrap::RtmpWrap()
        :_rtmp{nullptr}
{
    allocate();
}

RtmpWrap::~RtmpWrap()
{
    release();
}

void RtmpWrap::release()
{
    if (!_rtmp) {
        RTMP_Close(_rtmp);
        RTMP_Free(_rtmp);
        _rtmp = nullptr;
    }
}

void RtmpWrap::allocate()
{
    if (!_rtmp) {
        _rtmp = RTMP_Alloc();
    }
}

bool RtmpWrap::connect(const char* url)
{
    ALOGD("connect: url=%s", url);

    allocate();
    if (!_rtmp) {
        ALOGE("can not allocate rtmp, connect failed");
        return false;
    }

    int ret = 0;
    ret = RTMP_SetupURL(_rtmp, (const_cast<char*>(url)));
    if (ret!=TRUE) {
        ALOGE("connect: set url failed");
        RTMP_Free(_rtmp);
        _rtmp = nullptr;
        return false;
    }

    // enable writeable
    RTMP_EnableWrite(_rtmp);

    ret = RTMP_Connect(_rtmp, nullptr);
    if (ret!=TRUE) {
        ALOGE("connect: connect server failed");
        RTMP_Free(_rtmp);
        _rtmp = nullptr;
        return false;
    }

    ret = RTMP_ConnectStream(_rtmp, 0);
    if (ret!=TRUE) {
        ALOGE("connect: connect server stream failed");
        RTMP_Close(_rtmp);
        RTMP_Free(_rtmp);
        _rtmp = nullptr;
        return false;
    }

    ALOGI("connect: connect success");

    return true;
}

int RtmpWrap::sendH264Packet(uint8_t* data, size_t size, bool isKeyFrame, uint32_t timestamp)
{
    if (data==nullptr && size<11) {
        ALOGE("sendH264Packet: bad data");
        return -1;
    }

    auto* body = (uint8_t*) malloc(size+9);
    memset(body, 0x00, size+9);

    int ret = 0;
    int i = 0;
    if (isKeyFrame) {
        body[i++] = 0x17; // FrameType  CodecID
        body[i++] = 0x01; // AVCPacketType 1: AVC NALU

        //CompositionTime
        body[i++] = 0x00;
        body[i++] = 0x00;
        body[i++] = 0x00;

        // NALU size
        body[i++] = size >> 24 & 0xff;
        body[i++] = size >> 16 & 0xff;
        body[i++] = size >> 8 & 0xff;
        body[i++] = size >> 0 & 0xff;

        // NALU data
        memcpy(&body[i], data, size);
    }
    else {
        body[i++] = 0x27; // 2:Pframe  7:AVC
        body[i++] = 0x01; // AVC NALU

        //CompositionTime
        body[i++] = 0x00;
        body[i++] = 0x00;
        body[i++] = 0x00;

        // NALU size
        body[i++] = size >> 24 & 0xff;
        body[i++] = size >> 16 & 0xff;
        body[i++] = size >> 8 & 0xff;
        body[i++] = size >> 0 & 0xff;

        // NALU data
        memcpy(&body[i], data, size);
    }

    ret = sendPacket(RTMP_PACKET_TYPE_VIDEO, body, size+1, timestamp);
    free(body);
    return ret;
}

int RtmpWrap::sendVideoSpsPps(uint8_t* pps, size_t ppsLength, uint8_t* sps, size_t spsLength)
{
    RTMPPacket* pkt = nullptr;
    uint8_t* body = nullptr;
    int i = 0;

    pkt = (RTMPPacket*) malloc(RTMP_HEAD_SIZE+1024);
    memset(pkt, 0x00, RTMP_HEAD_SIZE+1024);
    pkt->m_body = (char*) pkt+RTMP_HEAD_SIZE;
    body = (uint8_t*) pkt->m_body;

    body[i++] = 0x17; //FrameType 和 CodecID
    //VideoData
    body[i++] = 0x00;   //AVCPacketType

    //CompositionTime
    body[i++] = 0x00;
    body[i++] = 0x00;
    body[i++] = 0x00;

    /*AVCDecoderConfigurationRecord*/
    body[i++] = 0x01;       //版本号 1
    body[i++] = sps[1];
    body[i++] = sps[2];
    body[i++] = sps[3];
    body[i++] = 0xff;


    // SPS
    body[i++] = 0xE1;
    //sps长度采用网络字节序存储
    body[i++] = (spsLength >> 8) & 0xff;
    body[i++] = (spsLength >> 0) & 0xff;
    memcpy(&body[i], sps, spsLength);
    i += spsLength;

    // PPS
    body[i++] = 0x01;
    //pps长度采用网络字节序存储
    body[i++] = (ppsLength >> 8) & 0xff;
    body[i++] = (ppsLength >> 0) & 0xff;
    memcpy(&body[i], pps, ppsLength);
    i += ppsLength;

    return 0;
}

int RtmpWrap::sendPacket(uint32_t type, uint8_t* data, size_t size, uint32_t timestamp)
{
    if (!_rtmp) {
        ALOGE("sendPacket: null rtmp object");
        return -1;
    }
    auto* pkt = (RTMPPacket*) malloc(RTMP_HEAD_SIZE+size);
    memset(pkt, 0x00, RTMP_HEAD_SIZE);

    pkt->m_body = (char*) pkt+RTMP_HEAD_SIZE;
    pkt->m_nBodySize = size;
    memcpy(pkt->m_body, data, size);

    pkt->m_hasAbsTimestamp = 0;
    pkt->m_packetType = type; // type is  audio or video
    pkt->m_nInfoField2 = _rtmp->m_stream_id;
    pkt->m_nChannel = 0x04; // ?
    pkt->m_headerType = RTMP_PACKET_SIZE_LARGE;
    if (type==RTMP_PACKET_TYPE_AUDIO && size!=4) {
        pkt->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
    }
    pkt->m_nTimeStamp = timestamp;

    int ret = 0;
    if (RTMP_IsConnected(_rtmp)) {
        // TRUE: 放进队列  FALSE 不放进队列
        ret = RTMP_SendPacket(_rtmp, pkt, TRUE);
    }

    // RTMPPacket_Free(pkt);
    free(pkt);

    return ret;
}

int RtmpWrap::sendAacSpecial(uint8_t* data, size_t length)
{
    return 0;
}

int RtmpWrap::sendAacData(uint8_t* data, size_t length, uint32_t timestamp)
{
    return 0;
}

}//namespace live
