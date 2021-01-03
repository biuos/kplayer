package dai.android.live.example

class RtmpPackage {
    var buffer: ByteArray? = null
    var type = 0
    var tms: Long = 0

    companion object {
        const val RTMP_PKT_TYPE_VIDEO = 0
    }
}
