package dai.android.live.example

import android.hardware.display.DisplayManager
import android.hardware.display.VirtualDisplay
import android.media.MediaCodec
import android.media.MediaCodecInfo
import android.media.MediaFormat
import android.media.projection.MediaProjection
import android.os.Bundle
import android.util.Log
import java.lang.Exception

class VideoCodec(private val screenLive: ScreenLive) {
    private val tag: String = "VideoCodec"

    private var mediaCodec: MediaCodec? = null
    private var timeStamp: Long = 0L
    private var startTime: Long = 0L
    private var mediaProjection: MediaProjection? = null
    private var virtualDisplay: VirtualDisplay? = null

    private val thread = Thread {
        if (null == mediaCodec) {
            Log.w(tag, "create MediaCodec first");
            return@Thread
        }

        val bufferInfo = MediaCodec.BufferInfo()
        mediaCodec!!.start()

        while (screenLive.started()) {
            // flush the Key frame
            if (timeStamp != 0L) {
                // every 2 seconds make a I key frame
                if (System.currentTimeMillis() - timeStamp >= 2_000) {
                    val params = Bundle();
                    params.putInt(MediaCodec.PARAMETER_KEY_REQUEST_SYNC_FRAME, 0)
                    mediaCodec!!.setParameters(params)
                }
            }
            timeStamp = System.currentTimeMillis()

            val index: Int = mediaCodec!!.dequeueOutputBuffer(bufferInfo, 10L)
            if (index > 0) {
                val buffer = mediaCodec!!.getOutputBuffer(index)
                val outData = ByteArray(bufferInfo.size)
                buffer?.get(outData)

                //
                if (startTime == 0L) {
                    startTime = bufferInfo.presentationTimeUs / 1000
                }

                val rtmpPkt = RtmpPackage()
                rtmpPkt.buffer = outData
                rtmpPkt.type = RtmpPackage.RTMP_PKT_TYPE_VIDEO
                rtmpPkt.tms = (bufferInfo.presentationTimeUs / 1000) - startTime
                screenLive.addPacket(rtmpPkt)
                mediaCodec?.releaseOutputBuffer(index, false)
            }
        }

        // stop codec
        startTime = 0L

        mediaCodec?.stop()
        mediaCodec?.release()
        mediaCodec = null

        if (null != virtualDisplay) {
            virtualDisplay?.release()
            virtualDisplay = null
        }

        if (null != mediaProjection) {
            mediaProjection?.stop()
            mediaProjection = null
        }
    }


    fun pushStart(projection: MediaProjection) {
        mediaProjection = projection

        val format = MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, 540, 960)
        format.setInteger(
            MediaFormat.KEY_COLOR_FORMAT,
            MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface
        )
        format.setInteger(MediaFormat.KEY_BIT_RATE, 400_000)
        format.setInteger(MediaFormat.KEY_FRAME_RATE, 35)
        format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 2)

        try {
            mediaCodec = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_VIDEO_AVC)
            mediaCodec?.configure(format, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE)
            val surface = mediaCodec?.createInputSurface()
            virtualDisplay = mediaProjection?.createVirtualDisplay(
                "screen-codec",
                540, 960, 1,
                DisplayManager.VIRTUAL_DISPLAY_FLAG_PUBLIC,
                surface, null, null
            )
        } catch (e: Exception) {
            Log.w(tag, "pushStart failed", e)
        }

        if (mediaCodec != null) {
            thread.start()
        }
    }
}
