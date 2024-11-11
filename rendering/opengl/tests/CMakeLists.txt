return_if_not_target( core::gfx )
add_boost_test(
    NAME core-gfx
    SOURCES
        tst_shader.cpp
        # test_program.cpp

    LINK
        core::gfx
        OSMesa::OSMesa
    )
