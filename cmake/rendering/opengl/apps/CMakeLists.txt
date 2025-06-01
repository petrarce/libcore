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

add_executable_test(NAME simple-uniform-test
    SOURCES simple_uniform_test.cpp
    LINK
        gfx::ui
        core::gfx
        rendering::utils
    RESOURCES
        simple_uniform_test/vert.glsl
        simple_uniform_test/frag.glsl
        simple_uniform_test/frag_diff.glsl
        simple_uniform_test/vert_diff.glsl
        simple_uniform_test/bricks.jpg

)

add_executable_test(NAME game-of-life
    SOURCE game_of_life.cpp
    LINK core::gfx

)
