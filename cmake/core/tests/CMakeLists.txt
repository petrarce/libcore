add_boost_test(
    NAME core-unit-tests
    SOURCES
        test_main.cpp
        test_math.cpp
        test_soundex.cpp
    LINK
        libcore::filters
        libcore::math
        absl::strings
        absl::flags

    )
