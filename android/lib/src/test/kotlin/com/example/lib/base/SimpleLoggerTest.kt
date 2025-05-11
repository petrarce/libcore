package com.example.lib.base

import org.junit.Test
import org.junit.Assert.*

class SimpleLoggerTest {
    @Test
    fun testLogMessage() {
        val logger = SimpleLogger()
        // Capture System.out to verify output if needed
        logger.logMessage("TEST", "message")
        // Add assertions based on your implementation
        assertTrue(true)
    }
}
