#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "BufferObjectBase.h"
#include <vector>
#include <glad/glad.h>

namespace core_gfx
{
namespace open_gl
{

template<class T>
concept IsBufferObject = std::is_base_of_v<BufferObjectBase, T>;
template<class T>
class MapBuffer;
template<GLenum Tgt>
class BufferObject;

/**
 * @brief Templated OpenGL buffer object wrapper
 *
 * @tparam Tgt The OpenGL buffer target (e.g. GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER)
 *
 * This class provides a type-safe interface for creating and managing OpenGL buffer objects.
 * It inherits from BufferObjectBase for basic buffer management and adds templated
 * data upload functionality.
 *
 * Features:
 * - Type-safe data upload via std::vector
 * - Configurable usage hints
 * - RAII resource management
 * - Supports mapping via MapBuffer friend class
 */
template<GLenum Tgt>
class BufferObject : public BufferObjectBase
{
	friend class MapBuffer<BufferObject<Tgt> >;

public:
	static const auto TargetValue = Tgt;
	template<typename T>
	explicit BufferObject(const std::vector<T>& data, GLuint accessHint = GL_STATIC_DRAW)
		: mAccessHint(accessHint)
	{
		glBindBuffer(Tgt, m_id);
		glBufferData(Tgt, data.size() * sizeof(T), data.data(), mAccessHint);
	}

private:
	GLuint mAccessHint{ 0 };
};

/**
 * @brief RAII wrapper for mapping OpenGL buffer objects
 *
 * @tparam T BufferObject type that meets InBufferObject concept requirements
 *
 * This class provides safe, scoped access to mapped buffer memory:
 * - Automatically maps buffer on construction
 * - Unmaps buffer on destruction (via RAII)
 * - Supports different access types via template parameter
 *
 * Usage example:
 * @code
 * {
 *     MapBuffer<GL_WRITE_ONLY> mapped(buffer); // Maps buffer
 *     // Access mapped data via mapped.RawPointer()
 * } // Automatically unmaps when out of scope
 * @endcode
 */
template<class T>
class MapBuffer
{
	static_assert(IsBufferObject<T>);

public:
	explicit MapBuffer(T& buffer, GLuint accessHint = GL_READ_ONLY)
		: mBuffer(buffer)
	{
		glBindBuffer(T::TargetValue, mBuffer.m_id);
		mBufferPtr = glMapBuffer(T::TargetValue, accessHint);
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

using ShaderStorageBuffer = BufferObject<GL_SHADER_STORAGE_BUFFER>;
static_assert(IsBufferObject<ShaderStorageBuffer>);
static_assert(!IsBufferObject<int>);

} // namespace open_gl
} // namespace core_gfx

#endif // BUFFER_OBJECT_H
