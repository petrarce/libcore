set( PYBIND11_FINDPYTHON ON )
find_package( pybind11 CONFIG )

return_if( NOT pybind11_FOUND )

pybind11_add_module(example_python example.cpp)
add_library( example::python ALIAS example_python )
target_link_libraries(example_python PUBLIC
    fmt::fmt
    networking
)
