add_executable(euler-project-solution
    main.cpp
)
target_link_libraries(euler-project-solution
    PRIVATE
        Boost::program_options
        fmt::fmt)
