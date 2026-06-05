package com.example.gesturehelper

import android.app.Activity
import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.graphics.PixelFormat
import android.media.projection.MediaProjectionManager
import android.os.Build
import android.os.IBinder
import android.util.Log
import android.view.Gravity
import android.view.View
import android.view.WindowManager
import androidx.compose.material3.MaterialTheme
import androidx.compose.ui.platform.ComposeView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.LifecycleRegistry
import androidx.lifecycle.ViewModelStore
import androidx.lifecycle.ViewModelStoreOwner
import androidx.lifecycle.setViewTreeLifecycleOwner
import androidx.lifecycle.setViewTreeViewModelStoreOwner
import androidx.savedstate.SavedStateRegistryController
import androidx.savedstate.SavedStateRegistryOwner
import androidx.savedstate.setViewTreeSavedStateRegistryOwner
import com.example.gesturehelper.capture.DebugScreenshotProcessor
import com.example.gesturehelper.ui.FloatingButtonContent
import com.example.lib.capture.ScreenCaptureManager
import com.example.lib.capture.ScreenCaptureResult
import com.example.lib.capture.ScreenshotProcessor
import com.example.lib.overlay.FloatingOverlayManager

class GestureHelperService : Service(), LifecycleOwner {
    private lateinit var overlayManager: FloatingOverlayManager
    private lateinit var captureManager: ScreenCaptureManager
    private lateinit var processor: ScreenshotProcessor

	private class ViewLifecycleOwner : LifecycleOwner, ViewModelStoreOwner, SavedStateRegistryOwner {
		override val lifecycle = LifecycleRegistry(this)
		override val viewModelStore = ViewModelStore()
		private val savedStateRegistryController = SavedStateRegistryController.create(this)
		override val savedStateRegistry = savedStateRegistryController.savedStateRegistry

		fun attachToView(view: View?){
			view?.setViewTreeLifecycleOwner(this)
			view?.setViewTreeViewModelStoreOwner(this)
			view?.setViewTreeSavedStateRegistryOwner(this)
		}
		fun onCreate(){
			savedStateRegistryController.performRestore(null)
			lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_CREATE)
		}
		fun onStart() {
			lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_START)

		}
		fun onResume() {
			lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_RESUME)
		}

		fun onStop() {
			lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_STOP)
		}

		fun onDestroy() {
			lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_DESTROY)
			viewModelStore.clear()
		}
	}
	private val viewLifecycleOwner = ViewLifecycleOwner();
	override val lifecycle: Lifecycle
		get() { return viewLifecycleOwner.lifecycle }

    override fun onCreate() {
        super.onCreate()
	    viewLifecycleOwner.onCreate()
        overlayManager = FloatingOverlayManager(
            getSystemService(Context.WINDOW_SERVICE) as WindowManager
        )
        processor = DebugScreenshotProcessor(this, overlayManager)
        createNotificationChannel()
    }

    override fun onStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
	    viewLifecycleOwner.onStart()
	    viewLifecycleOwner.onResume()

        val resultCode = intent?.getIntExtra(
            MainActivity.EXTRA_RESULT_CODE, Activity.RESULT_CANCELED
        ) ?: Activity.RESULT_CANCELED
        val data = if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            intent?.getParcelableExtra(MainActivity.EXTRA_DATA, Intent::class.java)
        } else {
            @Suppress("DEPRECATION")
            intent?.getParcelableExtra(MainActivity.EXTRA_DATA)
        }

        if (resultCode == Activity.RESULT_OK && data != null) {
            startForeground(NOTIFICATION_ID, buildNotification())
            val mpManager = getSystemService(MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
            val mediaProjection = mpManager.getMediaProjection(resultCode, data)
            captureManager = ScreenCaptureManager(this, mediaProjection)
            captureManager.startCapture()
            showFloatingButton()
        } else {
            stopSelf()
        }

        return START_NOT_STICKY
    }

    private fun showFloatingButton() {
        val composeView = ComposeView(this)
	    viewLifecycleOwner.attachToView(composeView)
        composeView.setContent {
            MaterialTheme {
                FloatingButtonContent(onTap = ::onButtonTap)
            }
        }

        val params = WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or
                    WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
            PixelFormat.TRANSLUCENT
        ).apply {
            gravity = Gravity.TOP or Gravity.START
            x = 16
            y = 100
        }

        overlayManager.add(composeView, params)
    }

    private fun onButtonTap() {
        if (!::captureManager.isInitialized) return
        when (val result = captureManager.captureFrame()) {
            is ScreenCaptureResult.Success -> processor.process(result.bitmap)
            is ScreenCaptureResult.Error -> Log.e(TAG, "Capture failed", result.exception)
            else -> {}
        }
    }

    override fun onDestroy() {
        viewLifecycleOwner.onStop()
        viewLifecycleOwner.onDestroy()
        if (::captureManager.isInitialized) captureManager.stopCapture()
        overlayManager.removeAll()
        super.onDestroy()
    }

    override fun onBind(intent: Intent?): IBinder? = null

    private fun createNotificationChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            val channel = NotificationChannel(
                CHANNEL_ID, "Gesture Helper", NotificationManager.IMPORTANCE_LOW
            ).apply { description = "Gesture helper is running" }
            val nm = getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
            nm.createNotificationChannel(channel)
        }
    }

    private fun buildNotification(): Notification {
        return Notification.Builder(this, CHANNEL_ID)
            .setContentTitle("Gesture Helper")
            .setContentText("Running")
            .setSmallIcon(android.R.drawable.ic_menu_camera)
            .setOngoing(true)
            .build()
    }

    companion object {
        private const val TAG = "GestureHelper"
        private const val CHANNEL_ID = "gesture_helper_channel"
        private const val NOTIFICATION_ID = 1
    }
}
