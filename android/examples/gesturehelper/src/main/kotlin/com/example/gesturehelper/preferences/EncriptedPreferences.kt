package com.example.gesturehelper.preferences

import android.content.Context
import androidx.datastore.core.DataStore
import androidx.datastore.preferences.core.Preferences
import androidx.datastore.preferences.core.stringPreferencesKey
import androidx.datastore.preferences.preferencesDataStore
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.SharingStarted
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.map
import kotlinx.coroutines.flow.stateIn
import kotlinx.coroutines.launch

internal val Context.dataStore: DataStore<Preferences> by preferencesDataStore(name = "preferences")

class EncriptedPreferences(
	private val context: Context,
) {
	val apiKeyFlow: StateFlow<String?>
		get() {
			return context.dataStore.data
				.map { prefs -> prefs[API_KEY] ?: "" }
				.stateIn(
					CoroutineScope(Dispatchers.Default),
					SharingStarted.Eagerly,
					null,
				)
		}

	fun updateApiKey(key: String) {
		CoroutineScope(Dispatchers.Default).launch {
			context.dataStore.updateData {
				it.toMutablePreferences().also { p ->
					p[API_KEY] = key
				}
			}
		}
	}

	companion object {
		private val API_KEY = stringPreferencesKey("api_key")
	}
}
