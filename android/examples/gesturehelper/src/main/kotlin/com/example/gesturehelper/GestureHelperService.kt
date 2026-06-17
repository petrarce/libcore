package com.example.gesturehelper

import android.app.Activity
import android.content.Context.WINDOW_SERVICE
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.PixelFormat
import android.media.projection.MediaProjectionManager
import android.os.Build
import android.util.Log
import android.view.Gravity
import android.view.View
import android.view.WindowManager
import androidx.compose.foundation.layout.Column
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.platform.ComposeView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner
import com.example.gesturehelper.preferences.EncriptedPreferences
import com.example.gesturehelper.ui.DraggableOverlaySurface
import com.example.gesturehelper.ui.FloatingToggleButton
import com.example.gesturehelper.ui.MessageToggleButton
import com.example.lib.capture.ScreenCaptureManager
import com.example.lib.capture.ScreenCaptureResult
import com.example.lib.overlay.FloatingOverlayManager
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class GestureHelperService :
	BackgroundServiceBase(),
	LifecycleOwner {
	private lateinit var overlayManager: FloatingOverlayManager
	private lateinit var captureManager: ScreenCaptureManager

	private val viewLifecycleOwner = ViewLifecycleOwner()
	private var capturedImage = mutableStateOf<ImageBitmap?>(null)

	var prefs = EncriptedPreferences(this)
	val aiChatClient = GeminiRequestRemplyProcessor()
	override val channelId = "gesture_helper_channel"
	override val channelName = "Gesture Helper"
	override val channelDescription = "Gesture helper is running"
	override val notificationId = 1
	override val notificationTitle = "Gesture Helper"
	override val notificationText = "Running"

	init {
		CoroutineScope(Dispatchers.Main).launch {
			prefs.apiKeyFlow.collect { aiChatClient.apiKey = it }
		}
	}

	override val lifecycle: Lifecycle
		get() {
			return viewLifecycleOwner.lifecycle
		}

	override fun onCreate() {
		super.onCreate()
		viewLifecycleOwner.onCreate()
		overlayManager =
			FloatingOverlayManager(
				getSystemService(WINDOW_SERVICE) as WindowManager,
			)
	}

	override fun onStartCommand(
		intent: Intent?,
		flags: Int,
		startId: Int,
	): Int {
		super.onStartCommand(intent, flags, startId)

		viewLifecycleOwner.onStart()
		viewLifecycleOwner.onResume()

		// Check if MediaProjection permission for screen capturing was granted
		val resultCode =
			intent?.getIntExtra(
				MainActivity.EXTRA_RESULT_CODE,
				Activity.RESULT_CANCELED,
			) ?: Activity.RESULT_CANCELED
		if (resultCode != Activity.RESULT_OK) {
			stopSelf()
			return START_NOT_STICKY
		}

		val data =
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
				intent?.getParcelableExtra(MainActivity.EXTRA_DATA, Intent::class.java)
			} else {
				@Suppress("DEPRECATION")
				intent?.getParcelableExtra(MainActivity.EXTRA_DATA)
			}

		if (data == null) {
			stopSelf()
			return START_NOT_STICKY
		}

		val mpManager = getSystemService(MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
		val mediaProjection = mpManager.getMediaProjection(resultCode, data)
		captureManager = ScreenCaptureManager(this, mediaProjection)
		buildUi()
		return START_NOT_STICKY
	}

	private fun addFloatingWindow(
		gravity: Int,
		content: @Composable (windowManager: WindowManager, params: WindowManager.LayoutParams, view: View) -> Unit,
	) {
		val composeView = ComposeView(this)
		viewLifecycleOwner.attachToView(composeView)
		val params =
			WindowManager
				.LayoutParams(
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
					PixelFormat.TRANSLUCENT,
				).apply {
					this.gravity = gravity
					this.x = 16
					this.y = 100
				}
		val windowManager = getSystemService(WINDOW_SERVICE) as WindowManager
		composeView.setContent {
			content(windowManager, params, composeView)
		}

		overlayManager.add(composeView, params)
	}

	private fun buildUi() {
		addFloatingWindow(
			gravity = Gravity.TOP or Gravity.START,
		) { windowManager, params, view ->
			Column {
				FloatingToggleButton(onToggle = {
					if (capturedImage.value == null) {
						onButtonTap()
					} else {
						capturedImage.value = null
					}
				})
				val image by remember { capturedImage }
				MessageToggleButton(windowManager, params, view, image)
				FloatingToggleButton(imageResource = R.drawable.ic_chess_pawn, onToggle = {
					// TODO: implement querying chess result
				})
			}
		}
		addFloatingWindow(
			gravity = Gravity.BOTTOM or Gravity.END,
		) { wm, p, v ->
			DraggableOverlaySurface(
				windowManager = wm,
				layoutParams = p,
				view = v,
				imageBitmap = capturedImage.value,
			)
		}
	}

	private fun onButtonTap() {
		Log.d(TAG, "Start capturing")
		if (!::captureManager.isInitialized) return
		when (val result = captureManager.captureFrame()) {
			is ScreenCaptureResult.Success -> {
				val image = result.image
				try {
					if (image.hardwareBuffer == null) {
						throw RuntimeException("Empty image after screen capture")
					}

					val bitmap = Bitmap.wrapHardwareBuffer(image.hardwareBuffer!!, null)
					capturedImage.value = bitmap?.copy(Bitmap.Config.ARGB_8888, false)?.asImageBitmap()
				} finally {
					image.close()
				}
			}
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

	companion object {
		private const val TAG = "GestureHelper"
	}
}
