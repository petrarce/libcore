#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "BufferObjectBase.h"
#include <vector>
#include <glad/glad.h>

namespace core_gfx {
namespace open_gl {

template<GLenum Target>
class BufferObject : public BufferObjectBase {
public:
    template<typename T>
    explicit BufferObject(const std::vector<T>& data);
};

template<GLenum Target>
template<typename T>
BufferObject<Target>::BufferObject(const std::vector<T>& data) {
    glBindBuffer(Target, m_id);
    glBufferData(Target, 
                data.size() * sizeof(T),
                data.data(),
                GL_STATIC_DRAW);
}

} // namespace open_gl
} // namespace core_gfx

#endif // BUFFER_OBJECT_H
