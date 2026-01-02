add_library(euler-project-solution-lib
    problem_26.cpp
    problem_26.h
    problem_27.cpp
    problem_27.h
)
target_link_libraries( euler-project-solution-lib
    PRIVATE
        absl::flat_hash_map
        fmt::fmt
        libcore::math
)

add_executable(euler-project-solution main.cpp )
target_link_libraries(euler-project-solution
    PRIVATE
        Boost::program_options
        euler-project-solution-lib)
