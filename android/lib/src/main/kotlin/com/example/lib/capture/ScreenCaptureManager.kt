package com.example.lib.capture

import android.content.Context
import android.graphics.PixelFormat
import android.hardware.display.DisplayManager
import android.hardware.display.VirtualDisplay
import android.media.ImageReader
import android.media.projection.MediaProjection
import android.util.DisplayMetrics
import android.util.Log
import android.view.WindowManager
import androidx.core.content.getSystemService

class ScreenCaptureManager(
	private val context: Context,
	private val mediaProjection: MediaProjection,
) {
	private var imageReader: ImageReader? = null
	private var virtualDisplay: VirtualDisplay? = null
	private val mpCallback = object : MediaProjection.Callback() {}

	init {
		when (val res = startCapture()) {
			is ScreenCaptureResult.Success -> Log.d(TAG, "Start Capturing")
			is ScreenCaptureResult.Error -> Log.d(TAG, "Unable to start capturing: ${res.exception}")
			else -> {}
		}
	}

	private fun startCapture(): ScreenCaptureResult =
		try {
			val metrics = displayMetrics()
			mediaProjection.registerCallback(mpCallback, null)

			imageReader =
				ImageReader.newInstance(
					metrics.widthPixels,
					metrics.heightPixels,
					PixelFormat.RGBA_8888,
					2,
				)
			virtualDisplay =
				mediaProjection.createVirtualDisplay(
					"CaptureDisplay",
					metrics.widthPixels,
					metrics.heightPixels,
					metrics.densityDpi,
					DisplayManager.VIRTUAL_DISPLAY_FLAG_AUTO_MIRROR,
					imageReader?.surface,
					null,
					null,
				)
			ScreenCaptureResult.SetupSuccess
		} catch (e: Exception) {
			ScreenCaptureResult.Error(e)
		}

	/**
	 * Captures the latest frame from the virtual display.
	 *
	 * @return [ScreenCaptureResult.Success] containing the raw [android.media.Image],
	 *   or [ScreenCaptureResult.Error] if capture fails.
	 *
	 * **Important:** The caller is responsible for closing the returned [android.media.Image]
	 *   via [android.media.Image.close] after consuming its data.
	 */
	fun captureFrame(): ScreenCaptureResult {
		val reader =
			imageReader
				?: return ScreenCaptureResult.Error(IllegalStateException("Capture not started"))
		val image =
			reader.acquireLatestImage()
				?: return ScreenCaptureResult.Error(IllegalStateException("No frame available"))
		return ScreenCaptureResult.Success(image)
	}

	fun stopCapture() {
		virtualDisplay?.release()
		imageReader?.close()
		virtualDisplay = null
		imageReader = null
		mediaProjection.unregisterCallback(mpCallback)
	}

	private fun displayMetrics(): DisplayMetrics {
		val wm = context.getSystemService<WindowManager>()!!
		val metrics = DisplayMetrics()
		wm.defaultDisplay.getRealMetrics(metrics)
		return metrics
	}

	companion object {
		private val TAG = "ScreenCaptureManager"
	}
}
