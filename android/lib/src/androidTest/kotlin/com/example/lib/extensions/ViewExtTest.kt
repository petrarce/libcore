package com.example.lib.extensions

import androidx.test.core.app.ApplicationProvider
import androidx.test.ext.junit.runners.AndroidJUnit4
import org.junit.Test
import org.junit.runner.RunWith
import android.view.View
import org.junit.Assert.*

@RunWith(AndroidJUnit4::class)
class ViewExtTest {
    @Test
    fun testViewVisibility() {
        val view = TestView(ApplicationProvider.getApplicationContext())
        view.hide()
        assertEquals(View.GONE, view.visibility)
    }
}
