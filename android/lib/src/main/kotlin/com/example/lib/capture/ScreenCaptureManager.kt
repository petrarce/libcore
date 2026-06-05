package com.example.lib.capture

import android.content.Context
import android.graphics.Bitmap
import android.graphics.PixelFormat
import android.hardware.display.DisplayManager
import android.hardware.display.VirtualDisplay
import android.media.Image
import android.media.ImageReader
import android.media.projection.MediaProjection
import android.util.DisplayMetrics
import android.view.WindowManager
import androidx.core.content.getSystemService

class ScreenCaptureManager(
	private val context: Context,
	private val mediaProjection: MediaProjection,
) {
	private var imageReader: ImageReader? = null
	private var virtualDisplay: VirtualDisplay? = null

	fun startCapture(): ScreenCaptureResult =
		try {
			val metrics = displayMetrics()
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

	fun captureFrame(): ScreenCaptureResult {
		val reader =
			imageReader
				?: return ScreenCaptureResult.Error(IllegalStateException("Capture not started"))
		val image =
			reader.acquireLatestImage()
				?: return ScreenCaptureResult.Error(IllegalStateException("No frame available"))
		return try {
			ScreenCaptureResult.Success(imageToBitmap(image))
		} catch (e: Exception) {
			ScreenCaptureResult.Error(e)
		} finally {
			image.close()
		}
	}

	fun stopCapture() {
		virtualDisplay?.release()
		imageReader?.close()
		virtualDisplay = null
		imageReader = null
	}

	private fun imageToBitmap(image: Image): Bitmap {
		val buffer = image.planes[0].buffer
		val w = image.width
		val h = image.height
		val pixels = IntArray(w * h)
		buffer.rewind()
		buffer.asIntBuffer().get(pixels)
		for (i in pixels.indices) {
			val p = pixels[i]
			val r = p and 0xff
			val g = (p ushr 8) and 0xff
			val b = (p ushr 16) and 0xff
			val a = (p ushr 24) and 0xff
			pixels[i] = (a shl 24) or (r shl 16) or (g shl 8) or b
		}
		return Bitmap.createBitmap(pixels, w, h, Bitmap.Config.ARGB_8888)
	}

	private fun displayMetrics(): DisplayMetrics {
		val wm = context.getSystemService<WindowManager>()!!
		val metrics = DisplayMetrics()
		wm.defaultDisplay.getRealMetrics(metrics)
		// test if ktlint actually works
		return metrics
	}
}
