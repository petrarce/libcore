package com.example.lib.lichess

import com.example.lib.lichess.model.CloudEvalResult
import com.example.lib.lichess.model.Pv
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import okhttp3.HttpUrl
import okhttp3.OkHttpClient
import okhttp3.Request
import org.json.JSONObject
import java.util.LinkedHashMap

/**
 * Client for the Lichess cloud evaluation API.
 *
 * Provides board evaluation using Lichess's cloud analysis engine.
 * Responses are cached in an LRU cache keyed by FEN + multiPv value.
 *
 * @param client The [OkHttpClient] to use for HTTP requests. Defaults to a client
 *   that sets a descriptive [User-Agent] header.
 * @param maxCacheSize Maximum number of evaluations to keep in the LRU cache.
 */
class LichessApiClient(
	private val client: OkHttpClient = createDefaultClient(),
	private val maxCacheSize: Int = 64,
) {
	private val cache =
		object : LinkedHashMap<String, CloudEvalResult>(maxCacheSize, 0.75f, true) {
			override fun removeEldestEntry(eldest: MutableMap.MutableEntry<String, CloudEvalResult>): Boolean =
				size > maxCacheSize
		}

	private val cacheLock = Any()

	/**
	 * Evaluates a chess position using the Lichess cloud-eval API.
	 *
	 * @param fen The FEN string of the position to evaluate.
	 * @param multiPv Number of principal variations to return (default 3).
	 * @return [Result.success] with [CloudEvalResult] on success,
	 *   [Result.failure] with [LichessApiException] on error.
	 */
	suspend fun EvaluateBoard(
		fen: String,
		multiPv: Int = 3,
	): Result<CloudEvalResult> {
		val cacheKey = "$fen|$multiPv"

		// Check cache
		synchronized(cacheLock) {
			cache[cacheKey]?.let { return Result.success(it) }
		}

		return withContext(Dispatchers.IO) {
			try {
				val url =
					HttpUrl
						.Builder()
						.scheme("https")
						.host("lichess.org")
						.addPathSegments("api/cloud-eval")
						.addQueryParameter("fen", fen)
						.addQueryParameter("multiPv", multiPv.toString())
						.build()

				val request = Request.Builder().url(url).build()
				val response = client.newCall(request).execute()
				val body = response.body?.string()

				if (!response.isSuccessful || body == null) {
					return@withContext Result.failure(
						LichessApiException(
							"HTTP ${response.code}: ${response.message}",
						),
					)
				}

				val json = JSONObject(body)
				val result = parseCloudEvalResponse(json)

				// Update cache
				synchronized(cacheLock) {
					cache[cacheKey] = result
				}

				Result.success(result)
			} catch (e: Exception) {
				Result.failure(LichessApiException("Failed to evaluate board", e))
			}
		}
	}

	private fun parseCloudEvalResponse(json: JSONObject): CloudEvalResult {
		val pvsArray = json.getJSONArray("pvs")
		val pvs =
			(0 until pvsArray.length()).map { i ->
				val pvJson = pvsArray.getJSONObject(i)
				Pv(
					moves = pvJson.getString("moves"),
					cp = if (pvJson.has("cp")) pvJson.getInt("cp") else null,
					mate = if (pvJson.has("mate")) pvJson.getInt("mate") else null,
				)
			}
		return CloudEvalResult(
			fen = json.getString("fen"),
			depth = json.getInt("depth"),
			knodes = json.getInt("knodes"),
			pvs = pvs,
		)
	}

	private companion object {
		private const val BASE_URL = "https://lichess.org/api/cloud-eval"
		private const val USER_AGENT =
			"GestureHelper/1.0 (Android; com.example.gesturehelper)"

		private fun createDefaultClient(): OkHttpClient =
			OkHttpClient
				.Builder()
				.addInterceptor { chain ->
					chain.proceed(
						chain
							.request()
							.newBuilder()
							.header("User-Agent", USER_AGENT)
							.build(),
					)
				}.build()
	}
}

/**
 * Exception thrown when a Lichess API request fails.
 *
 * @param message Human-readable error description.
 * @param cause Optional underlying cause (network error, parse failure, etc.).
 */
class LichessApiException(
	message: String,
	cause: Throwable? = null,
) : Exception(message, cause)
