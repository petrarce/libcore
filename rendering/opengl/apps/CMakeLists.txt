add_executable(test1 test1.cpp)
target_link_libraries(test1
    PUBLIC
        gfx::ui
        core::gfx
    )

add_executable_test(NAME test2-vertex-buffer
    SOURCES test2.cpp
    LINK
        gfx::ui
        core::gfx
    RESOURCES
        test2/frag.glsl
        test2/vert.glsl
)
