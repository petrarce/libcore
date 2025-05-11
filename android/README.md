# Android Libraries

This directory contains reusable libraries and components for Android app development using Kotlin and the Android framework.

## Purpose

The libraries here are designed to:
- Provide common functionality needed across multiple Android apps
- Encapsulate best practices for Android development
- Reduce boilerplate code in app projects
- Maintain consistent implementations of common patterns

## Contents

### Core Components
- **Base classes** - Common Activity/Fragment/ViewModel base classes
- **DI modules** - Shared dependency injection configurations
- **Extensions** - Kotlin extension functions for Android framework classes

### UI Components
- **Custom views** - Reusable UI widgets and custom views
- **Theming** - Shared theming and styling resources
- **Navigation** - Common navigation patterns and utilities

### Utility Libraries
- **Network** - HTTP client wrappers and API helpers  
- **Persistence** - Shared database and preferences utilities
- **Logging** - Centralized logging configuration
- **Analytics** - Common analytics event tracking

## Usage

To use these libraries in an Android project:

1. Add the desired module as a dependency in your app's `build.gradle`:
```kotlin
implementation(project(":android:logging"))
```

2. Import and use the components in your Kotlin code:
```kotlin
import com.example.android.logging.Logger
```

## Contribution Guidelines

When adding new libraries:
- Follow Android/Kotlin best practices
- Keep dependencies minimal
- Document all public APIs
- Include unit tests
- Support dependency injection
