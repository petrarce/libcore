package com.example.gesturehelper.ui

import android.util.Log
import android.view.View
import android.view.WindowManager
import androidx.compose.animation.AnimatedVisibility
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
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
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.unit.dp
import com.example.gesturehelper.GestureHelperService
import com.example.gesturehelper.R
import com.google.genai.types.FinishReason

@Composable
fun MessageToggleButton(
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
							if (it == null || it.finishReason().knownEnum() != FinishReason.Known.STOP) {
								Log.w("MessageToggleButton", "Failed to receive responce: $it")
								onComplete()
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
							Icon(painterResource(R.drawable.ic_key), null)
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
