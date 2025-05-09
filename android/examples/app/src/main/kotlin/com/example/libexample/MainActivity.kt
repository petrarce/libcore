package com.example.libexample

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import com.example.lib.ui.CustomTextView
import com.example.lib.ui.compose.GreetingText
import com.example.lib.extensions.capitalizeFirstLetter

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            LibTheme {
                Column {
                    GreetingText("Android")
                    CustomTextView(this@MainActivity).apply {
                        text = "Sample Text".capitalizeFirstLetter()
                    }
                }
            }
        }
    }
}
