package com.example.lib.extensions

fun String.capitalizeFirstLetter(): String = replaceFirstChar { it.uppercase() }
