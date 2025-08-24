add_library( game-of-life
        src/GameOfLife.cpp
        src/GameOfLife.h)

target_link_libraries( game-of-life PUBLIC
        Eigen
        core::gfx
)

#add_executable_test(NAME game-of-life
#        SOURCE game_of_life.cpp
#        LINK core::gfx
#
#)
