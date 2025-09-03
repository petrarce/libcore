add_boost_test(
    NAME core-unit-tests
    SOURCES
        test_main.cpp
        test_math.cpp
        test_soundex.cpp
        test_two_sum_calculator.cpp
        test_linked_list_numbers_sum.cpp
        test_max_average_subarray.cpp
    LINK
        libcore::filters
        libcore::math
        libcore::leetcode-solutions
        absl::strings
        absl::flags
    )
