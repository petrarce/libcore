return_if_not_target( core::gfx )
add_boost_test(
    NAME core-gfx-test
    SOURCES
        main.cpp
        tst_shader.cpp
        test_program.cpp
        test_arraybuffertests.cpp
        test_compute_shader.cpp
    LINK
        core::gfx
        OSMesa::OSMesa
    RESOURCES
        shaders/vert.glsl
        shaders/frag.glsl
)
