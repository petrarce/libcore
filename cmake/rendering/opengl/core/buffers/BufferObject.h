#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "BufferObjectBase.h"
#include <vector>
#include <atomic>
#include <thread>
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
public:
	static constexpr GLenum TargetValue = Tgt;

	// Disallow copying
	BufferObject(const BufferObject&) = delete;
	BufferObject& operator=(const BufferObject&) = delete;

	// Allow moving
	BufferObject(BufferObject&& other) noexcept;
	BufferObject& operator=(BufferObject&& other) noexcept;

	template<typename T>
	explicit BufferObject(const std::vector<T>& data, GLenum accessHint = GL_STATIC_DRAW);

	template<typename T>
	void UpdateData(const std::vector<T>& data, GLintptr offset = 0);

	size_t GetSize() const noexcept;

private:
	friend class MapBuffer<BufferObject<Tgt> >;

	std::atomic<int> mMapRefCount{ 0 };
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
	explicit MapBuffer(T& buffer, GLenum accessHint = GL_READ_ONLY) noexcept;

	~MapBuffer() noexcept;

	// Disallow copying
	MapBuffer(const MapBuffer&) = delete;
	MapBuffer& operator=(const MapBuffer&) = delete;

	// Allow moving
	MapBuffer(MapBuffer&& other) noexcept;
	MapBuffer& operator=(MapBuffer&& other) noexcept;

	[[nodiscard]] void* RawPointer() const noexcept;
	template<typename U>
	[[nodiscard]] U* As() const noexcept;

private:
	T& mBuffer;
	void* mBufferPtr{ nullptr };
};
//================== BufferObject definitions ======================================================

template<GLenum Tgt>
BufferObject<Tgt>::BufferObject(BufferObject&& other) noexcept
	: BufferObjectBase(std::move(other))
	, mAccessHint(other.mAccessHint)
{
}
template<GLenum Tgt>
BufferObject<Tgt>& BufferObject<Tgt>::operator=(BufferObject&& other) noexcept
{
	if (this != &other)
	{
		BufferObjectBase::operator=(std::move(other));
		mAccessHint = other.mAccessHint;
	}
	return *this;
}
template<GLenum Tgt>
template<typename T>
BufferObject<Tgt>::BufferObject(const std::vector<T>& data, GLenum accessHint)
	: mAccessHint(accessHint)
{
	if (data.empty())
		return;
	glBindBuffer(Tgt, m_id);
	glBufferData(Tgt, data.size() * sizeof(T), data.data(), mAccessHint);
}
template<GLenum Tgt>
template<typename T>
void BufferObject<Tgt>::UpdateData(const std::vector<T>& data, GLintptr offset)
{
	if (data.empty())
		return;
	glBindBuffer(Tgt, m_id);
	glBufferSubData(Tgt, offset, data.size() * sizeof(T), data.data());
}
template<GLenum Tgt>
size_t BufferObject<Tgt>::GetSize() const noexcept
{
	GLint size = 0;
	glBindBuffer(Tgt, m_id);
	glGetBufferParameteriv(Tgt, GL_BUFFER_SIZE, &size);
	return static_cast<size_t>(size);
}

// ===============================MapBuffer definitions ============================================
template<class T>
MapBuffer<T>::MapBuffer(T& buffer, GLenum accessHint) noexcept : mBuffer(buffer)
{
	// Increment ref count first
	int oldCount = mBuffer.mMapRefCount.fetch_add(1, std::memory_order_acquire);

	if (oldCount > 0)
	{
		// Buffer already mapped - decrement count back
		mBuffer.mMapRefCount.fetch_sub(1, std::memory_order_release);
		throw std::runtime_error("Buffer is already mapped");
	}

	// Memory barrier to ensure all writes are visible
	std::atomic_thread_fence(std::memory_order_release);

	glBindBuffer(T::TargetValue, mBuffer.m_id);
	mBufferPtr = glMapBuffer(T::TargetValue, accessHint);

	if (!mBufferPtr)
	{
		mBuffer.mMapRefCount.fetch_sub(1, std::memory_order_release);
		throw std::runtime_error("Failed to map buffer");
	}
}
template<class T>
MapBuffer<T>::~MapBuffer() noexcept
{
	if (!mBufferPtr)
		return;

	// Decrement ref count
	int newCount = mBuffer.mMapRefCount.fetch_sub(1, std::memory_order_acquire) - 1;

	if (newCount == 0)
	{
		// Last mapping - unmap the buffer
		std::atomic_thread_fence(std::memory_order_release);
		glBindBuffer(T::TargetValue, mBuffer.m_id);
		glUnmapBuffer(T::TargetValue);
	}
}
template<class T>
MapBuffer<T>::MapBuffer(MapBuffer&& other) noexcept
	: mBuffer(other.mBuffer)
	, mBufferPtr(other.mBufferPtr)
{
	other.mBufferPtr = nullptr;
}
template<class T>
MapBuffer<T>& MapBuffer<T>::operator=(MapBuffer&& other) noexcept
{
	if (this == &other)
		return *this;

	mBuffer = other.mBuffer;
	mBufferPtr = other.mBufferPtr;
	other.mBufferPtr = nullptr;

	return *this;
}
template<class T>
void* MapBuffer<T>::RawPointer() const noexcept
{
	return mBufferPtr;
}
template<class T>
template<typename U>
U* MapBuffer<T>::As() const noexcept
{
	return static_cast<U*>(mBufferPtr);
}

// =====================================Convenience aliases definitions ============================
using ShaderStorageBuffer = BufferObject<GL_SHADER_STORAGE_BUFFER>;

// ========================================= Concept tests =========================================
static_assert(IsBufferObject<ShaderStorageBuffer>);
static_assert(!IsBufferObject<int>);

} // namespace open_gl
} // namespace core_gfx

#endif // BUFFER_OBJECT_H
