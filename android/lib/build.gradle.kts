plugins {
	id("com.android.library")
	id("org.jetbrains.kotlin.android")
	id("com.dorongold.task-tree")
	id("org.jlleitschuh.gradle.ktlint")
}

android {
	namespace = "com.example.lib"
	compileSdk = 34

	defaultConfig {
		minSdk = 24
		testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
		consumerProguardFiles("consumer-rules.pro")
	}

	testOptions {
		targetSdk = 34
	}
	lint {
		targetSdk = 34
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
	baseline.set(file("ktlint-baseline.xml"))
	filter {
		exclude("**/generated/**")
	}
}

dependencies {
	implementation(libs.android.core.ktx)
	implementation(libs.android.appcompat)
	implementation(libs.lifecycle.runtime)

	// Compose
	implementation(platform(libs.compose.bom))
	implementation(libs.bundles.compose)
	androidTestImplementation(platform(libs.compose.bom))

	// Testing
	testImplementation(libs.junit)
	androidTestImplementation(libs.androidx.test.ext)
	androidTestImplementation(libs.espresso.core)
}
