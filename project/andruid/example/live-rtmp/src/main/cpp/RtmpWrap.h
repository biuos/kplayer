#ifndef LIVE_RTMP_WRAP_INCLUDE_H
#define LIVE_RTMP_WRAP_INCLUDE_H

#include "librtmp/rtmp.h"

namespace live {

class RtmpWrap {
private:
    RTMP* _rtmp;

private:
    void allocate();

    int sendPacket(uint32_t type, uint8_t* data, size_t size, uint32_t timestamp);

public:
    RtmpWrap();
    ~RtmpWrap();

    bool connect(const char* url);

    int sendH264Packet(uint8_t* data, size_t size, bool isKeyFrame, uint32_t timestamp);

    int sendVideoSpsPps(uint8_t* pps, size_t ppsLength, uint8_t* sps, size_t spsLength);

    int sendAacSpecial(uint8_t* data, size_t length);
    int sendAacData(uint8_t* data, size_t length, uint32_t timestamp);

    void release();
};

}

#endif //LIVE_RTMP_WRAP_INCLUDE_H
