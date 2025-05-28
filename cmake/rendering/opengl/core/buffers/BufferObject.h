#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "BufferObjectBase.h"
#include <vector>
#include <glad/glad.h>

namespace core_gfx {
namespace open_gl {

template<class T>
concept InBufferObject = std::is_base_of_v<BufferObjectBase, T>;

// add documentation here, ai!
template<InBufferObject T>
class MapBuffer
{
public:

    template<GLuint AccessType = GL_READ_ONLY>
    explicit MapBuffer(T& buffer)
        : mBuffer(buffer)
    {
        glBindBuffer(T::TargetValue, mBuffer.m_id);
        mBufferPtr = glMapBuffer(T::TargetValue, AccessType);
    }
    ~MapBuffer()
    {
        if (mBufferPtr == nullptr)
            return;
        glBindBuffer(T::TargetValue, mBuffer.m_id);
        glUnmapBuffer(T::TargetValue);
    }
    void* RawPointer() const { return mBufferPtr; }
private:
    T& mBuffer;
    void* mBufferPtr = nullptr;
};

template<GLenum Tgt>
class BufferObject : public BufferObjectBase {
    friend MapBuffer<BufferObject<Tgt>>;
public:
    using TargetValue = Tgt;
public:
    template<GLuint T, GLuint AccessHint = GL_STATIC_DRAW>
    explicit BufferObject(const std::vector<T>& data)
    {
        glBindBuffer(Tgt, m_id);
        glBufferData(Tgt,
                     data.size() * sizeof(T),
                     data.data(),
                     AccessHint);
    }
};
} // namespace open_gl
} // namespace core_gfx

#endif // BUFFER_OBJECT_H
