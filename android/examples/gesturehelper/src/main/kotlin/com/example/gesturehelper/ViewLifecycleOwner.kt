package com.example.gesturehelper

import android.view.View
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner
import androidx.lifecycle.LifecycleRegistry
import androidx.lifecycle.ViewModelStore
import androidx.lifecycle.ViewModelStoreOwner
import androidx.lifecycle.setViewTreeLifecycleOwner
import androidx.lifecycle.setViewTreeViewModelStoreOwner
import androidx.savedstate.SavedStateRegistry
import androidx.savedstate.SavedStateRegistryController
import androidx.savedstate.SavedStateRegistryOwner
import androidx.savedstate.setViewTreeSavedStateRegistryOwner

/**
 * A lightweight, manually-managed lifecycle owner used for Compose views displayed in
 * floating (overlay) windows.
 *
 * This class provides a full [Lifecycle] (created via [LifecycleRegistry]), a [ViewModelStore],
 * and a [SavedStateRegistry] — all without requiring a Fragment or Activity. It is designed
 * for use with overlay ComposeViews that need proper lifecycle management but exist outside
 * the normal Android activity/fragment lifecycle.
 *
 * Usage:
 * ```
 * val lifecycleOwner = ViewLifecycleOwner()
 * lifecycleOwner.onCreate()
 * lifecycleOwner.onStart()
 * lifecycleOwner.onResume()
 * // ... use the view ...
 * lifecycleOwner.onStop()
 * lifecycleOwner.onDestroy()
 * ```
 *
 * @see LifecycleOwner
 * @see ViewModelStoreOwner
 * @see SavedStateRegistryOwner
 */
internal class ViewLifecycleOwner :
	LifecycleOwner,
	ViewModelStoreOwner,
	SavedStateRegistryOwner {
	/** The lifecycle registry that drives the lifecycle state of this owner. */
	override val lifecycle = LifecycleRegistry(this)

	/** The view model store associated with this lifecycle owner. */
	override val viewModelStore = ViewModelStore()

	private val savedStateRegistryController = SavedStateRegistryController.create(this)

	/** The saved state registry for preserving and restoring state. */
	override val savedStateRegistry: SavedStateRegistry = savedStateRegistryController.savedStateRegistry

	/**
	 * Attaches this lifecycle owner to the given [View] so that Compose and other
	 * lifecycle-aware components can discover it via the view tree.
	 *
	 * This sets the [LifecycleOwner], [ViewModelStoreOwner], and [SavedStateRegistryOwner]
	 * for the view's subtree.
	 *
	 * @param view The view to attach to; if null, this is a no-op.
	 */
	fun attachToView(view: View?) {
		view?.setViewTreeLifecycleOwner(this)
		view?.setViewTreeViewModelStoreOwner(this)
		view?.setViewTreeSavedStateRegistryOwner(this)
	}

	/**
	 * Transitions the lifecycle to the CREATED state.
	 *
	 * Must be called before the view is used. This also performs saved state restoration
	 * with a null bundle (no prior saved state).
	 */
	fun onCreate() {
		savedStateRegistryController.performRestore(null)
		lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_CREATE)
	}

	/**
	 * Transitions the lifecycle to the STARTED state.
	 *
	 * Should be called when the view becomes visible.
	 */
	fun onStart() {
		lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_START)
	}

	/**
	 * Transitions the lifecycle to the RESUMED state.
	 *
	 * Should be called when the view is fully visible and interactive.
	 */
	fun onResume() {
		lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_RESUME)
	}

	/**
	 * Transitions the lifecycle to the STARTED state (from RESUMED).
	 *
	 * Should be called when the view is no longer in the foreground.
	 */
	fun onStop() {
		lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_STOP)
	}

	/**
	 * Transitions the lifecycle to the DESTROYED state and clears the [ViewModelStore].
	 *
	 * Must be called when the lifecycle owner is no longer needed to prevent memory leaks.
	 */
	fun onDestroy() {
		lifecycle.handleLifecycleEvent(Lifecycle.Event.ON_DESTROY)
		viewModelStore.clear()
	}
}
