package dai.android.live.example

import android.app.Activity
import android.content.Intent
import android.media.projection.MediaProjection
import android.media.projection.MediaProjectionManager
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    private val tag: String = "MainActivity"

    private var mediaProjection: MediaProjection? = null
    private var mediaProjectionManager: MediaProjectionManager? = null
    private var screenLive: ScreenLive? = null

    private val clicked = View.OnClickListener { v ->
        if (v?.id == R.id.btnLiveStart) {
            startLive()
        } else if (v?.id == R.id.btnLiveComplete) {
            stopLive()
        }
    }


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        findViewById<Button>(R.id.btnLiveStart).setOnClickListener(clicked)
        findViewById<Button>(R.id.btnLiveComplete).setOnClickListener(clicked)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if (null == mediaProjectionManager) {
            Log.w(tag, "MediaProjectionManager null object")
            return
        }

        if (requestCode == MEDIA_PROJECT_REQUEST_CODE && resultCode == Activity.RESULT_OK && null != data) {
            mediaProjection = mediaProjectionManager!!.getMediaProjection(resultCode, data)
            if (null != mediaProjection) {
                screenLive = ScreenLive()
                screenLive?.startPush(BILI_LIVE_RTMP_URL, mediaProjection!!)
            }
        }
    }


    private fun startLive() {
        mediaProjectionManager =
            getSystemService(MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
        if (null != mediaProjectionManager) {
            val captureIntent = mediaProjectionManager!!.createScreenCaptureIntent()
            if (null != captureIntent) {
                startActivityForResult(captureIntent, MEDIA_PROJECT_REQUEST_CODE)
            }
        }
    }


    private fun stopLive() {
        screenLive?.stopPush()
    }


    companion object {
        private const val MEDIA_PROJECT_REQUEST_CODE = 100
        private const val BILI_LIVE_RTMP_URL =
            "rtmp://live-push.bilivideo.com/live-bvc/?streamname=live_522907740_44689574&key=4005e7f6505f20cf24ea21cefe7b5218&schedule=rtmp"

        init {
            System.loadLibrary("BBLive")
        }
    }
}