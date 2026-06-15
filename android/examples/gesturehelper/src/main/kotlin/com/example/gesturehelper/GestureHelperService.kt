package com.example.gesturehelper

import android.R.attr.contentDescription
import android.R.attr.visible
import android.app.Activity
import android.content.Context.WINDOW_SERVICE
import android.content.Intent
import android.graphics.Bitmap
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
import androidx.compose.foundation.background
import androidx.compose.foundation.clickable
import androidx.compose.foundation.gestures.detectDragGestures
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.collectAsState
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.focus.onFocusEvent
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.input.pointer.util.VelocityTracker
import androidx.compose.ui.input.pointer.util.addPointerInputChange
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.ComposeView
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.unit.Density
import androidx.compose.ui.unit.Velocity
import androidx.compose.ui.unit.dp
import androidx.core.content.ContextCompat.getSystemService
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner
import com.example.gesturehelper.preferences.EncriptedPreferences
import com.example.gesturehelper.ui.FloatingButtonContent
import com.example.lib.capture.ScreenCaptureManager
import com.example.lib.capture.ScreenCaptureResult
import com.example.lib.overlay.FloatingOverlayManager
import com.google.genai.types.FinishReason
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
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

	var prefs = EncriptedPreferences(this)
	val aiChatClient = GeminiRequestRemplyProcessor()
	override val channelId = "gesture_helper_channel"
	override val channelName = "Gesture Helper"
	override val channelDescription = "Gesture helper is running"
	override val notificationId = 1
	override val notificationTitle = "Gesture Helper"
	override val notificationText = "Running"

	init {
		CoroutineScope(Dispatchers.Main).launch {
			prefs.apiKeyFlow.collect { aiChatClient.apiKey = it }
		}
	}

	override val lifecycle: Lifecycle
		get() {
			return viewLifecycleOwner.lifecycle
		}

	override fun onCreate() {
		super.onCreate()
		viewLifecycleOwner.onCreate()
		overlayManager =
			FloatingOverlayManager(
				getSystemService(WINDOW_SERVICE) as WindowManager,
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
		addFloatingWindow(
			gravity = Gravity.TOP or Gravity.START,
		) { windowManager, params, view ->
			Column {
				FloatingToggleButton(onToggle = {
					if (capturedImage.value == null) {
						onButtonTap()
					} else {
						capturedImage.value = null
					}
				})
				val image by remember { capturedImage }
				MessageToggleButton(windowManager, params, view, image)
			}
		}
		addFloatingWindow(
			gravity = Gravity.BOTTOM or Gravity.END,
		) { wm, p, v ->
			DraggableOverlaySurface(
				windowManager = wm,
				layoutParams = p,
				view = v,
				imageBitmap = capturedImage.value,
			)
		}
	}

	private fun onButtonTap() {
		Log.d(TAG, "Start capturing")
		if (!::captureManager.isInitialized) return
		when (val result = captureManager.captureFrame()) {
			is ScreenCaptureResult.Success -> {
				val image = result.image
				try {
					if (image.hardwareBuffer == null) {
						throw RuntimeException("Empty image after screen capture")
					}

					val bitmap = Bitmap.wrapHardwareBuffer(image.hardwareBuffer!!, null)
					capturedImage.value = bitmap?.copy(Bitmap.Config.ARGB_8888, false)?.asImageBitmap()
				} finally {
					image.close()
				}
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
	FloatingButtonContent(image = R.drawable.ic_camera, description = "Make Screenshot", onTap = onToggle)
}

/**
 * An icon button that shows a [CircularProgressIndicator] when [running] is true.
 *
 * @param modifier The modifier for the button layout.
 * @param image The resource identifier for the icon drawable.
 * @param onClick The action to perform when the button is clicked.
 * @param running Whether the button is in a loading state.
 */
@Composable
private fun RunningIconButton(
	modifier: Modifier = Modifier,
	image: Int,
	onClick: () -> Unit,
	running: Boolean,
) {
	IconButton(
		modifier = modifier,
		onClick = onClick,
	) {
		Icon(
			painterResource(image),
			contentDescription = "Send Message",
		)
		if (running) {
			CircularProgressIndicator()
		}
	}
}

/**
 * A text input field with a send button for composing messages.
 *
 * Provides a [TextField] for message entry and a [RunningIconButton] for sending.
 * Notifies the parent of focus changes via [onFocusChange] so that window-level
 * focus flags can be managed externally. Clears the input text when the field
 * becomes invisible.
 *
 * @param modifier The modifier for the root container.
 * @param visible Whether the input field is visible (controls reset on hide).
 * @param onFocusChange Callback invoked when the TextField gains or loses focus.
 *   Receives `true` when focused, `false` when unfocused.
 * @param onSend Callback invoked when the send button is clicked. Receives the
 *   message text and a completion callback to signal the operation is done.
 */
@Composable
private fun MessageInputField(
	modifier: Modifier = Modifier,
	visible: Boolean,
	onFocusChange: ((Boolean) -> Unit)? = null,
	onSend: (text: String, onComplete: () -> Unit) -> Unit,
	onChengeApiKeyClicked: () -> Unit,
) {
	var request by remember { mutableStateOf(TextFieldValue("")) }
	var requestInProgress by remember { mutableStateOf(false) }

	LaunchedEffect(visible) {
		if (!visible) {
			request = TextFieldValue("")
		}
	}

	Box(modifier = modifier) {
		TextField(
			modifier =
				Modifier
					.width(300.dp)
					.heightIn(max = 80.dp)
					.onFocusEvent { fs ->
						onFocusChange?.invoke(fs.isFocused)
					},
			value = request,
			onValueChange = { request = it },
		)

		RunningIconButton(
			modifier = Modifier.align(Alignment.TopEnd),
			image = R.drawable.ic_send,
			onClick = {
				requestInProgress = true
				onSend(request.text) {
					requestInProgress = false
				}
			},
			running = requestInProgress,
		)
		IconButton(
			modifier = Modifier.align(Alignment.TopStart),
			onClick = onChengeApiKeyClicked,
		) {
			Icon(painterResource(R.drawable.ic_chat), null)
		}
	}
}

@Composable
private fun MessageToggleButton(
	windowManager: WindowManager,
	params: WindowManager.LayoutParams,
	view: View,
	image: ImageBitmap? = null,
) {
	var textFieldVisible by remember { mutableStateOf(false) }
	var reply by remember { mutableStateOf<String?>(null) }
	var toggleChangeApiKey by remember { mutableStateOf(false) }
	val ghContext = LocalContext.current as GestureHelperService
	val apiKey by ghContext.prefs.apiKeyFlow.collectAsState("")
	var apiKeyTextField by remember { mutableStateOf(TextFieldValue("")) }
	var requestFocused by remember { mutableStateOf(false) }
	var apiKeyFocused by remember { mutableStateOf(false) }
	val HandlerFocused = { focused: Boolean ->
		if (focused) {
			windowManager.updateViewLayout(
				view,
				params.apply {
					flags = params.flags and WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE.inv()
				},
			)
		} else {
			windowManager.updateViewLayout(
				view,
				params.apply {
					flags = params.flags or WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE
				},
			)
		}
	}
	LaunchedEffect(requestFocused, apiKeyFocused) { HandlerFocused(requestFocused || apiKeyFocused) }

	val service = LocalContext.current as? GestureHelperService

	Row {
		FloatingButtonContent(
			image = R.drawable.ic_chat,
			description = "Chat With AI",
			onTap = { textFieldVisible = !textFieldVisible },
		)

		AnimatedVisibility(
			visible = textFieldVisible,
			enter = fadeIn(),
			exit = fadeOut(),
		) {
			Column {
				MessageInputField(
					modifier = Modifier,
					visible = textFieldVisible,
					onFocusChange = { isFocused ->
						apiKeyFocused = isFocused
					},
					onSend = { text, onComplete ->
						Log.e("MessageToggleButton", "Send Message!!!")
						service?.aiChatClient?.GenerateReply(text, image, handleResponce = {
							if (it.finishReason().knownEnum() != FinishReason.Known.STOP) {
								Log.w("MessageToggleButton", "Failed to receive responce: $it")
								return@GenerateReply
							}
							Log.w("MessageToggleButton", "Reply: $it")
							reply = it.text()
							onComplete()
						})
					},
					onChengeApiKeyClicked = {
						toggleChangeApiKey = !toggleChangeApiKey
					},
				)
				AnimatedVisibility(toggleChangeApiKey) {
					Box {
						TextField(
							modifier =
								Modifier.onFocusEvent({ state ->
									apiKeyFocused = state.isFocused
								}),
							value = apiKeyTextField,
							label = { Text(text = "Api key: ${apiKey ?: ""}") },
							onValueChange = {
								apiKeyTextField = it
							},
						)
						IconButton(
							modifier = Modifier.align(Alignment.TopEnd),
							onClick = {
								ghContext.prefs.updateApiKey(apiKeyTextField.text)
							},
						) {
							Icon(painterResource(R.drawable.ic_send), null)
						}
					}
				}
				AnimatedVisibility(reply != null) {
					Surface(
						modifier =
							Modifier
								.fillMaxWidth()
								.height(200.dp)
								.clickable(true, onClick = { reply = null })
								.verticalScroll(rememberScrollState()),
						color = Color.DarkGray.copy(alpha = 0.3f),
					) {
						Text(
							modifier = Modifier.fillMaxSize(),
							text = reply ?: "",
							color = Color.White,
						)
					}
				}
			}
		}
	}
}

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
private fun DraggableSurface(
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
private fun DraggableOverlaySurface(
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
