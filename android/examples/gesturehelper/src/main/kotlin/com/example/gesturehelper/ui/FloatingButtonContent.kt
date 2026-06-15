package com.example.gesturehelper.ui

import androidx.annotation.DrawableRes
import androidx.compose.foundation.background
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.size
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.material3.Icon
import androidx.compose.material3.MaterialTheme
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.draw.shadow
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.unit.dp

@Composable
fun FloatingButtonContent(
	modifier: Modifier = Modifier,
	@DrawableRes image: Int,
	description: String = "Buttont desctiption",
	onTap: () -> Unit,
) {
	Box(
		modifier =
			modifier
				.size(56.dp)
				.shadow(4.dp, CircleShape)
				.clip(CircleShape)
				.background(MaterialTheme.colorScheme.primaryContainer)
				.clickable(onClick = onTap),
		contentAlignment = Alignment.Center,
	) {
		Icon(
			painter = painterResource(image),
			contentDescription = description,
			tint = MaterialTheme.colorScheme.onPrimaryContainer,
		)
	}
}
