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

	// Disallow copying
	BufferObjectBase(const BufferObjectBase&) = delete;
	BufferObjectBase& operator=(const BufferObjectBase&) = delete;

	// Disallow moving
	BufferObjectBase(BufferObjectBase&&) = delete;
	BufferObjectBase& operator=(BufferObjectBase&&) = delete;

protected:
	GLuint m_id;
};

} // namespace open_gl
} // namespace core_gfx

#endif // BUFFER_OBJECT_BASE_H
