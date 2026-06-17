package com.example.gesturehelper.ui

import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.heightIn
import androidx.compose.foundation.layout.width
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.focus.onFocusEvent
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.input.TextFieldValue
import androidx.compose.ui.unit.dp
import com.example.gesturehelper.R

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
fun MessageInputField(
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
