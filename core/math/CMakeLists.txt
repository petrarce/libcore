add_library( math INTERFACE
    math.h
    gcd.xpp
)
target_include_directories( math INTERFACE . )

add_library( libcore::math ALIAS math )
