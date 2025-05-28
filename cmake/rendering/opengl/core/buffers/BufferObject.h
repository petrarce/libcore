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
	static constexpr GLenum TargetValue = Tgt;

	// Disallow copying
	BufferObject(const BufferObject&) = delete;
	BufferObject& operator=(const BufferObject&) = delete;

	// Allow moving
	BufferObject(BufferObject&& other) noexcept
		: BufferObjectBase(std::move(other))
		, mAccessHint(other.mAccessHint)
	{
	}

	BufferObject& operator=(BufferObject&& other) noexcept
	{
		if (this != &other)
		{
			BufferObjectBase::operator=(std::move(other));
			mAccessHint = other.mAccessHint;
		}
		return *this;
	}

	template<typename T>
	explicit BufferObject(const std::vector<T>& data, GLenum accessHint = GL_STATIC_DRAW)
		: mAccessHint(accessHint)
	{
		if (data.empty())
		{
			throw std::invalid_argument("Cannot create buffer with empty data");
		}
		glBindBuffer(Tgt, m_id);
		glBufferData(Tgt, data.size() * sizeof(T), data.data(), mAccessHint);
	}

	template<typename T>
	void UpdateData(const std::vector<T>& data, GLintptr offset = 0)
	{
		if (data.empty())
		{
			throw std::invalid_argument("Cannot update buffer with empty data");
		}
		glBindBuffer(Tgt, m_id);
		glBufferSubData(Tgt, offset, data.size() * sizeof(T), data.data());
	}

	size_t GetSize() const noexcept
	{
		GLint size = 0;
		glBindBuffer(Tgt, m_id);
		glGetBufferParameteriv(Tgt, GL_BUFFER_SIZE, &size);
		return static_cast<size_t>(size);
	}

private:
	GLenum mAccessHint{ GL_STATIC_DRAW };
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
	static_assert(IsBufferObject<T>, "Template parameter must be a BufferObject type");

public:
	explicit MapBuffer(T& buffer, GLenum accessHint = GL_READ_ONLY)
		: mBuffer(buffer)
	{
		glBindBuffer(T::TargetValue, mBuffer.m_id);
		mBufferPtr = glMapBuffer(T::TargetValue, accessHint);
		if (!mBufferPtr)
		{
			throw std::runtime_error("Failed to map buffer");
		}
	}

	~MapBuffer() noexcept
	{
		if (mBufferPtr)
		{
			glBindBuffer(T::TargetValue, mBuffer.m_id);
			glUnmapBuffer(T::TargetValue);
		}
	}

	// Disallow copying
	MapBuffer(const MapBuffer&) = delete;
	MapBuffer& operator=(const MapBuffer&) = delete;

	// Allow moving
	MapBuffer(MapBuffer&& other) noexcept
		: mBuffer(other.mBuffer)
		, mBufferPtr(other.mBufferPtr)
	{
		other.mBufferPtr = nullptr;
	}

	MapBuffer& operator=(MapBuffer&& other) noexcept
	{
		if (this != &other)
		{
			mBuffer = other.mBuffer;
			mBufferPtr = other.mBufferPtr;
			other.mBufferPtr = nullptr;
		}
		return *this;
	}

	[[nodiscard]] void* RawPointer() const noexcept { return mBufferPtr; }

	template<typename U>
	[[nodiscard]] U* As() const noexcept
	{
		return static_cast<U*>(mBufferPtr);
	}

private:
	T& mBuffer;
	void* mBufferPtr{ nullptr };
};

using ShaderStorageBuffer = BufferObject<GL_SHADER_STORAGE_BUFFER>;
static_assert(IsBufferObject<ShaderStorageBuffer>);
static_assert(!IsBufferObject<int>);

} // namespace open_gl
} // namespace core_gfx

#endif // BUFFER_OBJECT_H
