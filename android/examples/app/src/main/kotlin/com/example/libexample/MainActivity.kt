package com.example.libexample

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.Column
import androidx.compose.runtime.Composable
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.platform.LocalContext
import com.example.lib.ui.compose.GreetingText
import com.example.lib.ui.theme.LibcoreTheme

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            LibcoreTheme {
                Column {
                    GreetingText("Android")
                }
            }
        }
    }
}

@Preview(showSystemUi = true)
@Composable
fun PreviewView() {
    LibcoreTheme {
        Column {
            GreetingText("Android")
        }
    }
}
