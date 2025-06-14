#ifndef BUFFER_OBJECT_BASE_H
#define BUFFER_OBJECT_BASE_H

#include <glad/glad.h>

namespace core_gfx
{
namespace open_gl
{

class BufferObjectBase
{
public:
	BufferObjectBase();
	virtual ~BufferObjectBase();

	// Delete copy semantics
	BufferObjectBase(const BufferObjectBase&) = delete;
	BufferObjectBase& operator=(const BufferObjectBase&) = delete;

	// Implement move semantics
	BufferObjectBase(BufferObjectBase&& other) noexcept;
	BufferObjectBase& operator=(BufferObjectBase&& other) noexcept;

protected:
	GLuint m_id{ 0 };
};

} // namespace open_gl
} // namespace core_gfx

#endif // BUFFER_OBJECT_BASE_H
