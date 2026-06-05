package com.example.lib.overlay

import android.view.View
import android.view.WindowManager

class FloatingOverlayManager(
	private val windowManager: WindowManager,
) {
	private val overlays = mutableSetOf<View>()

	fun add(
		view: View,
		params: WindowManager.LayoutParams,
	) {
		windowManager.addView(view, params)
		overlays.add(view)
	}

	fun remove(view: View) {
		if (overlays.remove(view)) {
			try {
				windowManager.removeViewImmediate(view)
			} catch (_: IllegalArgumentException) {
			}
		}
	}

	fun removeAll() {
		overlays.toList().forEach { remove(it) }
	}
}
