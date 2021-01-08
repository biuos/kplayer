package dai.android.live.example.input

class AudioGather(private val cb: Callback?) {

    interface Callback {
        fun audioRawData(data: ByteArray);
    }


    fun start() {}
    fun stop() {}
    fun release() {}
}
