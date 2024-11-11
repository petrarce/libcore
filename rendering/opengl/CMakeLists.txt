find_package( glm )
find_package( glfw3 )
find_package ( OSMesa )

return_if( NOT glm_FOUND )
return_if( NOT glfw3_FOUND )
return_if( NOT OSMesa_FOUND )

add_subdirectory(glad/glad/)
add_subdirectory(core)
add_subdirectory(apps)
add_subdirectory(tests)
