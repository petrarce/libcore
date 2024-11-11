add_library(core-gfx SHARED
    Shader.cpp
    Shader.h
    Program.cpp
    Program.h
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
