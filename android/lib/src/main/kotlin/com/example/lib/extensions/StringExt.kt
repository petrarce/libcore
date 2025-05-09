package com.example.lib.extensions

fun String.capitalizeFirstLetter(): String {
    return replaceFirstChar { it.uppercase() }
}
