package com.example.lib.extensions

import android.graphics.Bitmap

fun Bitmap.scaledToFit(maxWidth: Int, maxHeight: Int): Bitmap {
    val scale = minOf(
        maxWidth.toFloat() / width,
        maxHeight.toFloat() / height,
        1f
    )
    if (scale >= 1f) return this
    return Bitmap.createScaledBitmap(
        this, (width * scale).toInt(), (height * scale).toInt(), true
    )
}
