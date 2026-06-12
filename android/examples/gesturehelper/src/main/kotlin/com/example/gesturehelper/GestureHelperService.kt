package com.example.gesturehelper

import android.R.attr.contentDescription
import android.R.attr.visible
import android.app.Activity
import android.content.Context
import android.content.Context.WINDOW_SERVICE
import android.content.Intent
import android.graphics.PixelFormat
import android.media.projection.MediaProjectionManager
import android.os.Build
import android.util.Log
import android.view.Gravity
import android.view.View
import android.view.WindowManager
import androidx.compose.animation.AnimatedVisibility
import androidx.compose.animation.core.Animatable
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.animation.splineBasedDecay
import androidx.compose.foundation.Image
import androidx.compose.foundation.gestures.detectDragGestures
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.input.pointer.util.VelocityTracker
import androidx.compose.ui.input.pointer.util.addPointerInputChange
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.ComposeView
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.Density
import androidx.compose.ui.unit.Velocity
import androidx.compose.ui.unit.dp
import androidx.core.content.ContextCompat.getSystemService
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner
import com.example.gesturehelper.ui.FloatingButtonContent
import com.example.lib.capture.ScreenCaptureManager
import com.example.lib.capture.ScreenCaptureResult
import com.example.lib.overlay.FloatingOverlayManager
import kotlinx.coroutines.coroutineScope
import kotlinx.coroutines.launch
import kotlin.math.roundToInt

class GestureHelperService :
	BackgroundServiceBase(),
	LifecycleOwner {
	private lateinit var overlayManager: FloatingOverlayManager
	private lateinit var captureManager: ScreenCaptureManager

	private val viewLifecycleOwner = ViewLifecycleOwner()
	private var capturedImage = mutableStateOf<ImageBitmap?>(null)

	override val channelId = "gesture_helper_channel"
	override val channelName = "Gesture Helper"
	override val channelDescription = "Gesture helper is running"
	override val notificationId = 1
	override val notificationTitle = "Gesture Helper"
	override val notificationText = "Running"

	override val lifecycle: Lifecycle
		get() {
			return viewLifecycleOwner.lifecycle
		}

	override fun onCreate() {
		super.onCreate()
		viewLifecycleOwner.onCreate()
		overlayManager =
			FloatingOverlayManager(
				getSystemService(Context.WINDOW_SERVICE) as WindowManager,
			)
	}

	override fun onStartCommand(
		intent: Intent?,
		flags: Int,
		startId: Int,
	): Int {
		super.onStartCommand(intent, flags, startId)

		viewLifecycleOwner.onStart()
		viewLifecycleOwner.onResume()

		// Check if MediaProjection permission for screen capturing was granted
		val resultCode =
			intent?.getIntExtra(
				MainActivity.EXTRA_RESULT_CODE,
				Activity.RESULT_CANCELED,
			) ?: Activity.RESULT_CANCELED
		if (resultCode != Activity.RESULT_OK) {
			stopSelf()
			return START_NOT_STICKY
		}

		val data =
			if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
				intent?.getParcelableExtra(MainActivity.EXTRA_DATA, Intent::class.java)
			} else {
				@Suppress("DEPRECATION")
				intent?.getParcelableExtra(MainActivity.EXTRA_DATA)
			}

		if (data == null) {
			stopSelf()
			return START_NOT_STICKY
		}

		val mpManager = getSystemService(MEDIA_PROJECTION_SERVICE) as MediaProjectionManager
		val mediaProjection = mpManager.getMediaProjection(resultCode, data)
		captureManager = ScreenCaptureManager(this, mediaProjection)
		buildUi()
		return START_NOT_STICKY
	}

	private fun addFloatingWindow(
		gravity: Int,
		content: @Composable (windowManager: WindowManager, params: WindowManager.LayoutParams, view: View) -> Unit,
	) {
		val composeView = ComposeView(this)
		viewLifecycleOwner.attachToView(composeView)
		val params =
			WindowManager
				.LayoutParams(
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.WRAP_CONTENT,
					WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
					WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE or WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
					PixelFormat.TRANSLUCENT,
				).apply {
					this.gravity = gravity
					this.x = 16
					this.y = 100
				}
		val windowManager = getSystemService(WINDOW_SERVICE) as WindowManager
		composeView.setContent {
			content(windowManager, params, composeView)
		}

		overlayManager.add(composeView, params)
	}

	private fun buildUi() {
		addFloatingWindow(Gravity.TOP or Gravity.START) { _, _, _ ->
			FloatingToggleButton(
				onToggle = {
					onButtonTap()
				},
			)
		}
		addFloatingWindow(Gravity.BOTTOM or Gravity.END) { wm, p, v ->
			DraggableOverlaySurface(
				windowManager = wm,
				layoutParams = p,
				view = v,
				imageBitmap = capturedImage.value,
			)
		}
	}

	private fun onButtonTap() {
		if (!::captureManager.isInitialized) return
		when (val result = captureManager.captureFrame()) {
			is ScreenCaptureResult.Success -> {
				capturedImage.value = result.bitmap.asImageBitmap()
			}
			is ScreenCaptureResult.Error -> Log.e(TAG, "Capture failed", result.exception)
			else -> {}
		}
	}

	override fun onDestroy() {
		viewLifecycleOwner.onStop()
		viewLifecycleOwner.onDestroy()
		if (::captureManager.isInitialized) captureManager.stopCapture()
		overlayManager.removeAll()
		super.onDestroy()
	}

	companion object {
		private const val TAG = "GestureHelper"
	}
}

/**
 * A toggle button displayed as a floating overlay window.
 *
 * Tapping the button triggers [onToggle], which typically controls the visibility
 * of the draggable surface overlay.
 */
@Composable
private fun FloatingToggleButton(onToggle: () -> Unit) {
	FloatingButtonContent(onTap = onToggle)
}

/**
 * A draggable overlay surface with animated visibility, positioned via [WindowManager].
 *
 * Displays a captured screenshot [imageBitmap] when available, or a solid blue surface
 * as a fallback. This composable manages horizontal drag gestures on a [Surface] and
 * updates the window position in real time. Visibility is controlled by the [visible]
 * parameter, with fade-in/fade-out transitions.
 *
 * @param windowManager The [WindowManager] used to update the window layout.
 * @param layoutParams The current [WindowManager.LayoutParams] of the overlay window.
 * @param view The [View] (ComposeView) being positioned.
 * @param visible Whether the surface should be visible (animated).
 * @param imageBitmap The captured screenshot to display, or null to show a placeholder.
 * @param modifier Optional [Modifier] for the surface.
 */
@Composable
private fun DraggableOverlaySurface(
	windowManager: WindowManager,
	layoutParams: WindowManager.LayoutParams,
	view: View,
	imageBitmap: ImageBitmap?,
	modifier: Modifier = Modifier,
) {
	val viewPosX = remember { Animatable(layoutParams.x.toFloat()) }
	val velocityTracker = remember { VelocityTracker() }
	val windowBounds = windowManager.currentWindowMetrics.bounds
	val maxX = (windowBounds.width() - view.width).coerceAtLeast(0)

	AnimatedVisibility(imageBitmap !== null, enter = fadeIn(), exit = fadeOut()) {
		LaunchedEffect(viewPosX.value, viewPosX.value) {
			layoutParams.apply { x = viewPosX.value.roundToInt() }
			windowManager.updateViewLayout(view, layoutParams)
		}
		Surface(
			shape = RoundedCornerShape(16.dp),
			modifier =
				modifier
					.size(100.dp, 200.dp)
					.pointerInput(Unit) {
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
			color = Color.Blue,
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

@Composable
fun MainView(
	modifier: Modifier = Modifier,
	onTap: () -> Unit,
) {
	Box(
		modifier =
			modifier.fillMaxSize(),
	) {
		FloatingButtonContent(
			modifier =
				Modifier
					.align(Alignment.TopStart),
			onTap = onTap,
		)
		Surface(
			modifier =
				Modifier
					.align(Alignment.BottomEnd)
					.size(10.dp, 20.dp),
			color = Color.Blue,
		) {
		}
	}
}

@Preview(showSystemUi = true)
@Composable
fun MainVeiwPreview() {
	MainView(modifier = Modifier.fillMaxSize(), {})
}
