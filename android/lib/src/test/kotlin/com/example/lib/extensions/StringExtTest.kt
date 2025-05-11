package com.example.lib.extensions

import org.junit.Test
import org.junit.Assert.*

class StringExtTest {
    @Test
    fun testCapitalizeFirstLetter() {
        assertEquals("Android", "android".capitalizeFirstLetter())
    }
}
