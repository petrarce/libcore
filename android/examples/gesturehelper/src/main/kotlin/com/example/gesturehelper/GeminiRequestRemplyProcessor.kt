package com.example.gesturehelper

import android.graphics.Bitmap
import android.util.Log
import androidx.compose.ui.graphics.ImageBitmap
import androidx.compose.ui.graphics.asAndroidBitmap
import com.google.genai.Client
import com.google.genai.types.Content
import com.google.genai.types.GenerateContentResponse
import com.google.genai.types.Part
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.ByteArrayOutputStream

class GeminiRequestRemplyProcessor {
	companion object {
		private val TAG = GeminiRequestRemplyProcessor::class.java.simpleName
	}

	private var client: Client? = null

	private var _apiKey: String? = null
	var apiKey: String?
		get() {
			return _apiKey
		}
		set(key) {
			Log.d("GeminiRequestRemplyProcessor", "Api key updated: $key")
			_apiKey = key
			if (_apiKey == null) {
				return
			}
			client = Client.Builder().apiKey(_apiKey).build()
		}

	fun GenerateReply(
		vararg contents: Any?,
		handleResponce: (responce: GenerateContentResponse?) -> Unit,
	) {
		CoroutineScope(Dispatchers.Default).launch {
			var partsArray = mutableListOf<Part>()
			for (content in contents) {
				if (content == null) {
					continue
				}
				when (content) {
					is String -> partsArray.add(Part.fromText(content))
					is ImageBitmap -> {
						val os = ByteArrayOutputStream()
						content.asAndroidBitmap().compress(Bitmap.CompressFormat.JPEG, 5, os)
						partsArray.add(Part.fromBytes(os.toByteArray(), "image/jpeg"))
					}
					else -> continue
				}
			}
			val requestContent = Content.fromParts(*partsArray.toTypedArray())
			val cl = client
			if (cl == null) {
				throw RuntimeException("Api key not configured")
			}
			val reply =
				try {
					cl.models.generateContent(
						"gemini-3.1-flash-lite",
						requestContent,
						null,
					)
				} catch (e: Exception) {
					Log.e(TAG, "Failed to make gemini request: ${e.message}")
					null
				}
			handleResponce(reply)
		}
	}
}
