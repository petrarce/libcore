add_library(core-gfx SHARED
    programs/Shader.cpp
    programs/Shader.h
    programs/Program.cpp
    programs/Program.h
    utils/Errors.h
    utils/Errors.cpp
    buffers/VertexArrayBuffer.h
    buffers/VertexArrayBuffer.cpp
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

target_compile_options(core-gfx PUBLIC -g -O0)
