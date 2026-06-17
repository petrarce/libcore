package com.example.gesturehelper.ui

import android.view.View
import android.view.WindowManager
import androidx.compose.animation.AnimatedVisibility
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.unit.dp

/**
 * A draggable overlay surface that displays a captured screenshot.
 *
 * Delegates to [DraggableSurface] for dragging mechanics and shows the
 * screenshot [imageBitmap] or a blue placeholder when null.
 *
 * @param windowManager The [WindowManager] used to update the window layout.
 * @param layoutParams The current [WindowManager.LayoutParams] of the overlay window.
 * @param view The [View] (ComposeView) being positioned.
 * @param imageBitmap The captured screenshot to display, or null to show a placeholder.
 * @param modifier Optional [Modifier] for the surface.
 */
@Composable
fun DraggableOverlaySurface(
	windowManager: WindowManager,
	layoutParams: WindowManager.LayoutParams,
	view: View,
	imageBitmap: ImageBitmap?,
	modifier: Modifier = Modifier,
) {
	AnimatedVisibility(imageBitmap !== null, enter = fadeIn(), exit = fadeOut()) {
		DraggableSurface(
			modifier =
				modifier
					.size(100.dp, 200.dp)
					.background(Color.Blue, RoundedCornerShape(16.dp)),
			windowManager = windowManager,
			layoutParams = layoutParams,
			view = view,
		) {
			if (imageBitmap != null) {
				Image(
					bitmap = imageBitmap,
					contentDescription = "Captured screenshot",
					modifier = Modifier.fillMaxSize(),
					contentScale = ContentScale.Fit,
				)
			}
		}
	}
}
