plugins {
	id("com.android.application")
	id("org.jetbrains.kotlin.android")
	id("org.jlleitschuh.gradle.ktlint")
}

android {
	namespace = "com.example.gesturehelper"
	compileSdk = 34

	defaultConfig {
		applicationId = "com.example.gesturehelper"
		minSdk = 31
		targetSdk = 34
		versionCode = 1
		versionName = "1.0"
	}

	buildTypes {
		release {
			isMinifyEnabled = false
			proguardFiles(
				getDefaultProguardFile("proguard-android-optimize.txt"),
				"proguard-rules.pro",
			)
		}
	}
	compileOptions {
		sourceCompatibility = JavaVersion.VERSION_17
		targetCompatibility = JavaVersion.VERSION_17
	}
	kotlinOptions {
		jvmTarget = "17"
	}
	buildFeatures {
		compose = true
	}
	composeOptions {
		kotlinCompilerExtensionVersion =
			libs.versions.compose.compiler
				.get()
	}
}

ktlint {
	debug.set(false)
	verbose.set(false)
	android.set(true)
	outputToConsole.set(true)
	outputColorName.set("RED")
	ignoreFailures.set(false)
	enableExperimentalRules.set(false)
	baseline.set(file("../ktlint-baseline.xml"))
	filter {
		exclude("**/generated/**")
	}
}

dependencies {
	implementation(project(":lib"))
	implementation(libs.android.core.ktx)
	implementation(libs.lifecycle.runtime)
	implementation(libs.lifecycle.process)
	implementation(libs.savedstate.ktx)
	implementation(libs.google.genai)
	implementation(platform(libs.compose.bom))
	implementation(libs.bundles.compose)
	implementation(libs.compose.ui.tooling.preview)
	debugImplementation(libs.compose.ui.tooling)
}
