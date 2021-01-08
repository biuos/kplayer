package dai.android.live.example

import android.Manifest
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.view.SurfaceView
import android.view.View
import android.view.Window
import android.view.WindowManager
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat


class LiveActivity : AppCompatActivity() {

    private var surfaceView: SurfaceView? = null
    private var buttonLiveOperator: Button? = null
    private var hasLiveStarted = false
    private var permissionsGranted = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // 设置全屏
        requestWindowFeature(Window.FEATURE_NO_TITLE)
        window.setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN
        )
        setContentView(R.layout.activity_live)

        surfaceView = findViewById(R.id.surfaceView)
        buttonLiveOperator = findViewById(R.id.btnLiveOperator)
        buttonLiveOperator?.setOnClickListener(buttonClicked)
    }

    override fun onResume() {
        super.onResume()
        surfaceView?.keepScreenOn = true
    }

    override fun onStart() {
        super.onStart()

        checkPermission()
    }

    override fun onStop() {
        super.onStop()
        surfaceView?.keepScreenOn = false
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == 1) {
            var isAllGranted = true
            for (grant in grantResults) {
                if (grant != PackageManager.PERMISSION_GRANTED) {
                    isAllGranted = false
                    break
                }
            }

            if (isAllGranted) {
                permissionsGranted = true
            }
        }
    }

    private fun startLive() {
    }

    private fun stopLive() {
    }

    private val buttonClicked = View.OnClickListener {
        when (it.id) {
            R.id.btnLiveOperator -> {
                if (!hasLiveStarted) {
                    startLive()
                    buttonLiveOperator?.text = getString(R.string.strLiveOperatorEnd)
                } else {
                    stopLive()
                    buttonLiveOperator?.text = getString(R.string.strLiveOperatorStart)
                }
            }
        }
    }

    private fun checkPermissionAllGranted(): Boolean {
        for (permission in permissions) {
            if (ContextCompat.checkSelfPermission(
                    this,
                    permission
                ) != PackageManager.PERMISSION_GRANTED
            ) {
                return false
            }
        }
        return true
    }

    private fun checkPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkPermissionAllGranted()) {
                permissionsGranted = true
                return
            }
            ActivityCompat.requestPermissions(this, permissions, 1)
        } else {
            permissionsGranted = true
        }
    }


    companion object {
        private val permissions = arrayOf(
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.CAMERA
        )
    }
}
