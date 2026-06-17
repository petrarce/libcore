package com.example.gesturehelper.ui

import androidx.annotation.DrawableRes
import androidx.compose.runtime.Composable
import com.example.gesturehelper.R

/**
 * A toggle button displayed as a floating overlay window.
 *
 * Tapping the button triggers [onToggle], which typically controls the visibility
 * of the draggable surface overlay.
 */
@Composable
fun FloatingToggleButton(@DrawableRes imageResource: Int = R.drawable.ic_camera, onToggle: () -> Unit) {
	FloatingButtonContent(image = imageResource, description = "Make Screenshot", onTap = onToggle)
}
