add_boost_test(
    NAME core-unit-tests
    SOURCES
        test_main.cpp
        test_math.cpp
        test_soundex.cpp
        test_two_sum_calculator.cpp
    LINK
        libcore::filters
        libcore::math
        libcore::leetcode-solutions
        absl::strings
        absl::flags
    )
