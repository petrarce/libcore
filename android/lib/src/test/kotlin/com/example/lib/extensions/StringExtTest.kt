package com.example.lib.extensions

import org.junit.Assert.*
import org.junit.Test

class StringExtTest {
	@Test
	fun testCapitalizeFirstLetter() {
		assertEquals("Android", "android".capitalizeFirstLetter())
	}
}
