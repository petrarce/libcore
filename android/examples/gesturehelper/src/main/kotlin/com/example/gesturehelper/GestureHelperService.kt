package com.example.gesturehelper

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
import androidx.compose.foundation.layout.Row
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asImageBitmap
import androidx.compose.ui.platform.ComposeView
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner
import com.example.gesturehelper.preferences.EncriptedPreferences
import com.example.gesturehelper.ui.ChessAnalysisButton
import com.example.gesturehelper.ui.DraggableOverlaySurface
import com.example.gesturehelper.ui.FloatingToggleButton
import com.example.gesturehelper.ui.MessageToggleButton
import com.example.lib.capture.ScreenCaptureManager
import com.example.lib.capture.ScreenCaptureResult
import com.example.lib.lichess.LichessApiClient
import com.example.lib.overlay.FloatingOverlayManager
import com.google.genai.types.FinishReason
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.suspendCancellableCoroutine
import kotlin.coroutines.resume

class GestureHelperService :
	BackgroundServiceBase(),
	LifecycleOwner {
	private lateinit var overlayManager: FloatingOverlayManager
	private lateinit var captureManager: ScreenCaptureManager

	private val viewLifecycleOwner = ViewLifecycleOwner()
	private var capturedImage = mutableStateOf<ImageBitmap?>(null)

	var prefs = EncriptedPreferences(this)
	val aiChatClient = GeminiRequestRemplyProcessor()
	private val lichessClient = LichessApiClient()
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
			Row {
				FloatingToggleButton(onToggle = {
					if (capturedImage.value == null) {
						onButtonTap()
					} else {
						capturedImage.value = null
					}
				})
				val image by remember { capturedImage }
				MessageToggleButton(windowManager, params, view, image)
				ChessAnalysisButton(onAnalyze = { onResult ->
					onChessAnalysisTap(onResult)
				})
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

	private fun captureScreenshot(): ImageBitmap? {
		Log.d(TAG, "Capturing screenshot for chess analysis")
		if (!::captureManager.isInitialized) return null
		return when (val result = captureManager.captureFrame()) {
			is ScreenCaptureResult.Success -> {
				val image = result.image
				try {
					if (image.hardwareBuffer == null) {
						Log.e(TAG, "Empty image after screen capture")
						return null
					}
					val bitmap = Bitmap.wrapHardwareBuffer(image.hardwareBuffer!!, null)
					bitmap?.copy(Bitmap.Config.ARGB_8888, false)?.asImageBitmap()
				} finally {
					image.close()
				}
			}
			is ScreenCaptureResult.Error -> {
				Log.e(TAG, "Capture failed", result.exception)
				null
			}
			else -> null
		}
	}

	private fun onChessAnalysisTap(onResult: (String) -> Unit) {
		val imageBitmap =
			captureScreenshot() ?: run {
				Log.e(TAG, "Failed to capture screenshot for chess analysis")
				onResult("")
				return
			}

		CoroutineScope(Dispatchers.Default).launch {
			// Step 1: Ask AI to detect chessboard and return FEN
			val fenPrompt =
				"""
				Look at this screenshot. If there is a chess board visible on the screen,
				provide ONLY the FEN string of the position shown (e.g. rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1).
				If no chess board is detected, respond with exactly: NO_CHESSBOARD
				""".trimIndent()

			val fen =
				suspendCancellableCoroutine<String?> { cont ->
					aiChatClient.GenerateReply(fenPrompt, imageBitmap, handleResponce = { response ->
						if (response == null || response.finishReason().knownEnum() != FinishReason.Known.STOP) {
							cont.resume(null)
							return@GenerateReply
						}
						cont.resume(response.text()?.trim())
					})
				}

			if (fen == null || fen == "NO_CHESSBOARD") {
				Log.w(TAG, "No chessboard detected in screenshot: fen=$fen")
				onResult("")
				return@launch
			}

			Log.d(TAG, "FEN detected: $fen")

			// Step 2: Evaluate with Lichess cloud API
			val lichessResult = lichessClient.EvaluateBoard(fen)

			lichessResult.fold(
				onSuccess = { eval ->
					Log.d(TAG, "Lichess evaluation: depth=${eval.depth}, pvs=${eval.pvs.size}")

					// Step 3: Ask AI to beautify and explain the Lichess results
					val pvSummaries =
						eval.pvs
							.mapIndexed { index, pv ->
								val score = if (pv.mate != null) "Mate in ${pv.mate}" else "${pv.cp}cp"
								"${index + 1}. ${pv.moves} ($score)"
							}.joinToString("\n")

					val beautifyPrompt =
						"""
						Chess position evaluation:
						FEN: ${eval.fen}
						Depth: ${eval.depth}
						Principal variations:
						$pvSummaries

						Provide a brief 1-2 sentence explanation of all suggested moves and why it is good,
						including relevant chess theory. Keep it concise.
						""".trimIndent()

					val explanation =
						suspendCancellableCoroutine<String?> { cont ->
							aiChatClient.GenerateReply(beautifyPrompt, handleResponce = { response ->
								if (response == null || response.finishReason().knownEnum() != FinishReason.Known.STOP) {
									cont.resume("Analysis complete.")
									return@GenerateReply
								}
								cont.resume(response.text())
							})
						}

					onResult(explanation ?: "Analysis complete.")
				},
				onFailure = { error ->
					Log.e(TAG, "Lichess evaluation failed: ${error.message}", error)
					onResult("")
				},
			)
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
