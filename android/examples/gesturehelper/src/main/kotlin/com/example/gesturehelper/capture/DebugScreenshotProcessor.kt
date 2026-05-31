package com.example.gesturehelper.capture

import android.content.Context
import android.graphics.Bitmap
import android.graphics.PixelFormat
import android.os.Handler
import android.os.Looper
import android.view.Gravity
import android.view.View
import android.view.WindowManager
import android.widget.ImageView
import com.example.lib.capture.ScreenshotProcessor
import com.example.lib.extensions.scaledToFit
import com.example.lib.overlay.FloatingOverlayManager

class DebugScreenshotProcessor(
    private val context: Context,
    private val overlayManager: FloatingOverlayManager,
) : ScreenshotProcessor {

    private val handler = Handler(Looper.getMainLooper())
    private var currentPreview: View? = null
    private var dismissRunnable: Runnable? = null

    override fun process(bitmap: Bitmap) {
        handler.post {
            dismissCurrent()
            showPreview(bitmap)
        }
    }

    private fun showPreview(bitmap: Bitmap) {
        val scaled = bitmap.scaledToFit(PREVIEW_MAX_W, PREVIEW_MAX_H)
        val imageView = ImageView(context).apply {
            setImageBitmap(scaled)
            setOnClickListener { dismissCurrent() }
            setBackgroundColor(0xFF000000.toInt())
            alpha = 0.9f
        }

        val params = WindowManager.LayoutParams(
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.WRAP_CONTENT,
            WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
            WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or
                    WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
            PixelFormat.TRANSLUCENT
        ).apply {
            gravity = Gravity.BOTTOM or Gravity.END
            x = 16
            y = 16
        }

        overlayManager.add(imageView, params)
        currentPreview = imageView

        val runnable = Runnable { dismissCurrent() }
        dismissRunnable = runnable
        handler.postDelayed(runnable, DISMISS_DELAY_MS)
    }

    private fun dismissCurrent() {
        currentPreview?.let { overlayManager.remove(it) }
        currentPreview = null
        dismissRunnable?.let { handler.removeCallbacks(it) }
        dismissRunnable = null
    }

    private companion object {
        const val PREVIEW_MAX_W = 360
        const val PREVIEW_MAX_H = 640
        const val DISMISS_DELAY_MS = 10_000L
    }
}
