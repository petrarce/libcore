package com.example.gesturehelper.ui

import androidx.compose.animation.AnimatedVisibility
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.unit.dp
import com.example.gesturehelper.R

/**
 * A button that triggers chess board analysis from a screenshot.
 *
 * When tapped, invokes [onAnalyze] with a callback to deliver the result text.
 * Shows a loading indicator while analysis is in progress and displays the
 * result in a scrollable surface once available, similar to [MessageToggleButton].
 *
 * @param onAnalyze Callback invoked when the button is tapped. Receives an [onResult]
 *   callback that should be called with the analysis result text (or empty string on failure).
 */
@Composable
fun ChessAnalysisButton(onAnalyze: (onResult: (String) -> Unit) -> Unit) {
	var reply by remember { mutableStateOf<String?>(null) }
	var isLoading by remember { mutableStateOf(false) }

	Row {
		FloatingButtonContent(
			image = R.drawable.ic_chess_pawn,
			description = "Analyze chess position",
			onTap = {
				if (!isLoading) {
					isLoading = true
					onAnalyze { result ->
						isLoading = false
						reply = if (result.isNotEmpty()) result else null
					}
				}
			},
		)

		if (isLoading) {
			Box(
				modifier =
					Modifier
						.size(56.dp)
						.padding(16.dp),
				contentAlignment = Alignment.Center,
			) {
				CircularProgressIndicator(
					modifier = Modifier.size(24.dp),
					color = Color.White,
				)
			}
		}

		AnimatedVisibility(
			visible = reply != null,
			enter = fadeIn(),
			exit = fadeOut(),
		) {
			Surface(
				modifier =
					Modifier
						.fillMaxWidth()
						.height(200.dp)
						.clickable { reply = null }
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
