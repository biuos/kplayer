package dai.android.live.example

import android.media.projection.MediaProjection
import android.util.Log
import java.util.concurrent.LinkedBlockingQueue
import java.util.concurrent.atomic.AtomicBoolean

class ScreenLive : ILivePush<MediaProjection> {
    private val tag: String = "ScreenLive"

    private val _pktQueue = LinkedBlockingQueue<RtmpPackage>()
    private var _mediaProjection: MediaProjection? = null
    private var _url: String? = null

    private val started: AtomicBoolean = AtomicBoolean(false)
    private val thread = Thread {
        if (_url.isNullOrEmpty()) {
            Log.w(tag, "null rtmp push url")
            return@Thread
        }

        if (null == _mediaProjection) {
            Log.w(tag, "create MediaProjection object first")
            return@Thread
        }

        Log.i(tag, "1- nativeConnect")
        if (!nativeConnect(_url!!)) {
            Log.e(tag, "1- nativeConnect")
            return@Thread
        }

        val videoCodec = VideoCodec(ScreenLive@ this)
        videoCodec.pushStart(_mediaProjection!!)

        while (started.get()) {
            var pkt: RtmpPackage? = null
            try {
                pkt = _pktQueue.take()
            } catch (e: InterruptedException) {
                Log.w(tag, "run:", e)
            }

            if (null == pkt) {
                Log.e(tag, "No rtmp package, will exit")
                break
            }

            if (pkt.buffer != null && pkt.buffer!!.isNotEmpty()) {
                nativeSendData(pkt.buffer!!, pkt.buffer!!.size, pkt.type, pkt.tms)
            }
        }

        started.set(false)

        Log.d(tag, "push live thread will exit");
    }


    override fun startPush(url: String, any: MediaProjection) {
        if (started.get()) {
            Log.d(tag, "current is started")
            return
        }

        _url = url
        _mediaProjection = any

        started.set(true)
        thread.start()
    }

    override fun stopPush() {
        started.set(false)
    }


    fun started(): Boolean {
        return started.get();
    }

    fun addPacket(pkt: RtmpPackage) {
        if (started()) {
            _pktQueue.add(pkt)
        }
    }


    private external fun nativeConnect(url: String): Boolean
    private external fun nativeSendData(data: ByteArray, len: Int, type: Int, tms: Long)
    private external fun nativeRelease()
}
