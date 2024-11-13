add_library(gfx-ui SHARED
    Ui.h
    Glfw.h Glfw.cpp
)

target_link_libraries(gfx-ui PUBLIC glfw)
target_include_directories(gfx-ui PUBLIC ../)

add_library(gfx::ui ALIAS gfx-ui)
