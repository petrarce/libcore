#include "BufferObjectBase.h"

namespace core_gfx
{
namespace open_gl
{

BufferObjectBase::BufferObjectBase() { glCreateBuffers(1, &m_id); }

BufferObjectBase::~BufferObjectBase() 
{ 
    if (m_id != 0) 
    {
        glDeleteBuffers(1, &m_id); 
    }
}

BufferObjectBase::BufferObjectBase(BufferObjectBase&& other) noexcept
    : m_id(other.m_id)
{
    other.m_id = 0; // Prevent the moved-from object from deleting the buffer
}

BufferObjectBase& BufferObjectBase::operator=(BufferObjectBase&& other) noexcept
{
    if (this != &other)
    {
        if (m_id != 0)
        {
            glDeleteBuffers(1, &m_id);
        }
        m_id = other.m_id;
        other.m_id = 0; // Prevent the moved-from object from deleting the buffer
    }
    return *this;
}

} // namespace open_gl
} // namespace core_gfx
