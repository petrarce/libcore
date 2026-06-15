package com.example.lib.capture

import android.media.Image

sealed class ScreenCaptureResult {
	data class Success(
		val image: Image,
	) : ScreenCaptureResult()

	data class Error(
		val exception: Throwable,
	) : ScreenCaptureResult()

	data object SetupSuccess : ScreenCaptureResult()
}
