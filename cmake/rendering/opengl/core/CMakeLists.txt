add_library(core-gfx SHARED
    programs/Shader.cpp
    programs/Shader.h
    programs/Program.cpp
    programs/Program.h
    utils/Errors.h
    utils/Errors.cpp
    buffers/VertexArrayBuffer.h
    buffers/VertexArrayBuffer.cpp
    buffers/TexturedVertexArrayBuffer.h
    buffers/TexturedVertexArrayBuffer.cpp
    buffers/SharedStorageBuffer.cpp
    buffers/SharedStorageBuffer.h
    buffers/BufferObjectBase.cpp
    buffers/BufferObjectBase.h
    buffers/BufferObject.h
        evaluators/ComputeEvaluator.cpp
        evaluators/ComputeEvaluator.h
)

target_link_libraries(core-gfx
    PUBLIC
        glm::glm
        glad::glad
        absl::strings
        absl::flags
        soil2
    )

add_library(core::gfx ALIAS core-gfx)

target_include_directories(core-gfx PUBLIC .)
