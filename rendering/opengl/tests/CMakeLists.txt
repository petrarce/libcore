return_if_not_target( core::gfx )
add_boost_test(
    NAME core-gfx
    SOURCES
        main.cpp
        tst_shader.cpp
        test_program.cpp

    LINK
        core::gfx
        OSMesa::OSMesa
    RESOURCES
        shaders/vert.glsl
        shaders/frag.glsl
    )

target_sources(core-gfx-test
  PRIVATE
    test_program.cpp
)
