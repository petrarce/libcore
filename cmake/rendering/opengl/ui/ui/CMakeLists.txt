add_library( gfx-ui SHARED
    Ui.h
    Glfw.h Glfw.cpp
)
add_library( gfx::ui ALIAS gfx-ui )

target_link_libraries( gfx-ui PUBLIC
    glfw
    rendering::utils
)
target_include_directories( gfx-ui PUBLIC ../ )
