#include "BufferObjectBase.h"

namespace core_gfx
{
namespace open_gl
{

BufferObjectBase::BufferObjectBase() { glCreateBuffers(1, &m_id); }

BufferObjectBase::~BufferObjectBase() { glDeleteBuffers(1, &m_id); }

} // namespace open_gl
} // namespace core_gfx
