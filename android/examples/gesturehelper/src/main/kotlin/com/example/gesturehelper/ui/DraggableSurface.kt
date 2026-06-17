package com.example.gesturehelper.ui

import android.view.View
import android.view.WindowManager
import androidx.compose.animation.core.Animatable
import androidx.compose.animation.splineBasedDecay
import androidx.compose.foundation.gestures.detectDragGestures
import androidx.compose.foundation.layout.Box
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.input.pointer.util.VelocityTracker
import androidx.compose.ui.input.pointer.util.addPointerInputChange
import androidx.compose.ui.unit.Density
import androidx.compose.ui.unit.Velocity
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.launch
import kotlin.math.roundToInt

/**
 * A draggable surface overlay that can be repositioned via horizontal drag gestures.
 *
 * Handles drag gestures, fling/decay animation, and updates the [WindowManager] layout
 * position in real time. Visual customization (shape, color, size, etc.) should be
 * applied via [modifier] by the caller.
 *
 * @param modifier The modifier for the surface (use for shape, color, size, etc.).
 * @param windowManager The [WindowManager] used to update the window layout.
 * @param layoutParams The current [WindowManager.LayoutParams] of the overlay window.
 * @param view The [View] (ComposeView) being positioned.
 * @param content The composable content to display inside the surface.
 */
@Composable
fun DraggableSurface(
	modifier: Modifier = Modifier,
	windowManager: WindowManager,
	layoutParams: WindowManager.LayoutParams,
	view: View,
	content: @Composable () -> Unit,
) {
	val viewPosX = remember { Animatable(layoutParams.x.toFloat()) }
	val velocityTracker = remember { VelocityTracker() }
	val windowBounds = windowManager.currentWindowMetrics.bounds
	val maxX = (windowBounds.width() - view.width).coerceAtLeast(0)

	LaunchedEffect(viewPosX.value) {
		layoutParams.apply { x = viewPosX.value.roundToInt() }
		windowManager.updateViewLayout(view, layoutParams)
	}
	Box(
		modifier =
			modifier.pointerInput(Unit) {
				coroutineScope {
					detectDragGestures(
						onDragStart = {
							velocityTracker.resetTracking()
						},
						onDrag = { change, dragAmount ->
							launch {
								viewPosX.snapTo(
									(viewPosX.value - dragAmount.x).coerceIn(
										0f,
										maxX.toFloat(),
									),
								)
							}
							velocityTracker.addPointerInputChange(change)
							change.consume()
						},
						onDragEnd = {
							val velocity =
								velocityTracker.calculateVelocity(
									Velocity(
										1000f,
										1000f,
									),
								)
							launch {
								viewPosX.animateDecay(
									initialVelocity = -velocity.x,
									animationSpec = splineBasedDecay(Density(2f)),
								) {
									val clamped = value.coerceIn(0f, maxX.toFloat())
									if (clamped != value) {
										launch { snapTo(clamped) }
									}
								}
							}
						},
					)
				}
			},
	) {
		content()
	}
}
