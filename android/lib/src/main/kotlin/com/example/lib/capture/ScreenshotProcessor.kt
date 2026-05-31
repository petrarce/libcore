package com.example.lib.capture

import android.graphics.Bitmap

fun interface ScreenshotProcessor {
    fun process(bitmap: Bitmap)
}
