package com.example.lib.capture

import android.graphics.Bitmap

interface ScreenshotProcessor {
	fun process(bitmap: Bitmap)
}
