package com.example.gesturehelper

import android.app.Notification
import android.app.NotificationChannel
import android.app.NotificationManager
import android.app.Service
import android.content.Context
import android.content.Intent
import android.os.Build
import android.os.IBinder

/**
 * A convenience base class for foreground [Service] implementations.
 *
 * Handles notification channel creation and foreground service startup boilerplate,
 * allowing subclasses to focus on business logic rather than notification setup.
 *
 * Subclasses must provide notification configuration via the abstract properties
 * and implement [onHandleStartCommand] which replaces [onStartCommand].
 *
 * ## Usage
 * ```
 * class MyService : BackgroundServiceBase() {
 *     override val channelId = "my_channel"
 *     override val channelName = "My Service"
 *     override val channelDescription = "My service is running"
 *     override val notificationId = 1
 *
 *     override fun onHandleStartCommand(intent: Intent?, flags: Int, startId: Int): Int {
 *         startForegroundService()
 *         // ... do work ...
 *         return START_STICKY
 *     }
 * }
 * ```
 *
 * @see Service
 * @see Notification
 * @see NotificationChannel
 */
abstract class BackgroundServiceBase : Service() {
	// -- Notification configuration (must be provided by subclass) --

	/** Unique identifier for the notification channel. */
	protected abstract val channelId: String

	/** User-visible name for the notification channel. */
	protected abstract val channelName: String

	/** User-visible description for the notification channel. */
	protected abstract val channelDescription: String

	/** Unique notification ID for [startForeground]. */
	protected abstract val notificationId: Int

	// -- Optional overridable notification properties --

	/** Title shown in the foreground notification. */
	protected open val notificationTitle: String = "Service Running"

	/** Text shown in the foreground notification. */
	protected open val notificationText: String = "Running"

	/** Icon resource ID for the foreground notification. */
	protected open val notificationIcon: Int = android.R.drawable.ic_menu_camera

	/** Importance level for the notification channel (Android 8+). */
	protected open val notificationImportance: Int = NotificationManager.IMPORTANCE_LOW

	override fun onStartCommand(
		intent: Intent?,
		flags: Int,
		startId: Int,
	): Int {
		createNotificationChannel()
		startForeground(notificationId, buildNotification())
		return START_NOT_STICKY
	}

	override fun onBind(intent: Intent?): IBinder? = null

	private fun createNotificationChannel() {
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
			val channel =
				NotificationChannel(
					channelId,
					channelName,
					notificationImportance,
				).apply { description = channelDescription }
			val nm = getSystemService(Context.NOTIFICATION_SERVICE) as NotificationManager
			nm.createNotificationChannel(channel)
		}
	}

	private fun buildNotification(): Notification =
		Notification
			.Builder(this, channelId)
			.setContentTitle(notificationTitle)
			.setContentText(notificationText)
			.setSmallIcon(notificationIcon)
			.setOngoing(true)
			.build()
}
