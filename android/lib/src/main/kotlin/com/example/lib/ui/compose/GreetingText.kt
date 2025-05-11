package com.example.lib.ui.compose

import androidx.compose.material3.Text
import androidx.compose.runtime.Composable

@Composable
fun GreetingText(name: String) {
    Text(text = "Hello, $name!")
}
