add_library(camera SHARED
    camera.cpp
    camera/camera.h
)

target_include_directories(camera PUBLIC .)

add_library(camera::camera ALIAS camera)
