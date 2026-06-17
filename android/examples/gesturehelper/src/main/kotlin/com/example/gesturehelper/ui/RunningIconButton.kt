package com.example.gesturehelper.ui

import androidx.compose.material3.CircularProgressIndicator
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.painterResource
import com.example.gesturehelper.R

/**
 * An icon button that shows a [CircularProgressIndicator] when [running] is true.
 *
 * @param modifier The modifier for the button layout.
 * @param image The resource identifier for the icon drawable.
 * @param onClick The action to perform when the button is clicked.
 * @param running Whether the button is in a loading state.
 */
@Composable
fun RunningIconButton(
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
