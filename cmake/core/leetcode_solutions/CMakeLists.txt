add_library( leetcode-solutions
    two_sum_calculator.cpp
    two_sum_calculator.h
    linked_list_number_sums.cpp
    linked_list_number_sums.h
        max_number_of_viwels.cpp
        max_number_of_viwels.h
        max_consecutive_ones.cpp
        max_consecutive_ones.h
        merge_strings_alternately.cpp
        merge_strings_alternately.h
        strings_gcd.cpp
        strings_gcd.h
        array_differences.cpp
        array_differences.h
        two_arrays_median.cpp
        two_arrays_median.h
)
add_library( libcore::leetcode-solutions ALIAS leetcode-solutions )
target_compile_options( leetcode-solutions PRIVATE -g -O0 )
