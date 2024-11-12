add_library(core-gfx SHARED
    Shader.cpp
    Shader.h
    Program.cpp
    Program.h
    utils/Errors.h
    utils/Errors.cpp
)

target_link_libraries(core-gfx
    PUBLIC
        glm::glm
        glad::glad
        absl::strings
        absl::flags
    )

add_library(core::gfx ALIAS core-gfx)

target_include_directories(core-gfx PUBLIC .)
