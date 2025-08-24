add_library(math INTERFACE math.h gcd.hpp
        ../utils/StrongTypedef.h)
target_include_directories(math INTERFACE .)

add_library(libcore::math ALIAS math)
