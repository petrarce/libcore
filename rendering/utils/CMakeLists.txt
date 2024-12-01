add_library(rendering-utils SHARED
    utils/Camera.cpp
    utils/Camera.h
)

add_library(rendering::utils ALIAS rendering-utils)

target_link_libraries(rendering-utils PRIVATE camera::camera)
target_include_directories(rendering-utils PUBLIC .)
