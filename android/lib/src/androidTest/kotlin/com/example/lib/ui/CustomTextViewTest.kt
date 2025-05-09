package com.example.lib.ui

import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.rule.ActivityTestRule
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class CustomTextViewTest {
    @get:Rule
    val activityRule = ActivityTestRule(MainActivity::class.java)

    @Test
    fun textView_displaysCorrectText() {
        onView(withId(R.id.custom_view))
            .check(matches(isDisplayed()))
    }
}
