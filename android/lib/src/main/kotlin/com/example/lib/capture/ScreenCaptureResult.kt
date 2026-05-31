package com.example.lib.capture

import android.graphics.Bitmap

sealed class ScreenCaptureResult {
    data class Success(val bitmap: Bitmap) : ScreenCaptureResult()
    data class Error(val exception: Throwable) : ScreenCaptureResult()
    data object SetupSuccess : ScreenCaptureResult()
}
