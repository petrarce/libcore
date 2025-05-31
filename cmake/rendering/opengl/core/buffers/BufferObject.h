#ifndef BUFFER_OBJECT_H
#define BUFFER_OBJECT_H

#include "BufferObjectBase.h"

#include <optional>
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
public:
	static constexpr GLenum TargetValue = Tgt;

	template<typename T>
	explicit BufferObject(const std::vector<T>& data, GLenum accessHint = GL_STATIC_DRAW);

	template<typename T>
	void UpdateData(const std::vector<T>& data, GLintptr offset = 0);
	template<class T>
	void LoadData(std::vector<T>& output);

	size_t GetSize() const noexcept;

private:
	friend class MapBuffer<BufferObject<Tgt> >;

	int mMapRefCount{ 0 };
	std::optional<void*> mCachedMappedPtr;
	GLenum mAccessHint{ GL_STATIC_DRAW };
};
/**
 * @brief RAII wrapper for mapping OpenGL buffer objects
 *
 * @tparam T BufferObject type that meets InBufferObject concept requirements
 *
 * @warning This class is NOT thread-safe. Mapping operations on the same buffer
 *          from different threads will lead to undefined behavior.
 * @note The buffer can be mapped multiple times in the same scope, but will only
 *       be actually unmapped when the last MapBuffer goes out of scope.
 *       The mapped pointer is cached and reused for subsequent mappings.
 *
 * This class provides scoped access to mapped buffer memory:
 * - Automatically maps buffer on construction
 * - Unmaps buffer on destruction (via RAII)
 * - Supports different access types via template parameter
 * - Must be used from a single thread per buffer object
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

	// Disallow moving
	MapBuffer(MapBuffer&& other) = delete;
	MapBuffer& operator=(MapBuffer&& other) = delete;

	[[nodiscard]] void* RawPointer() const noexcept;
	template<typename U>
	[[nodiscard]] U* As() const noexcept;

private:
	T& mBuffer;
};
//================== BufferObject definitions ======================================================

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
template<typename T>
void BufferObject<Tgt>::LoadData(std::vector<T>& output)
{
	assert(output.size() * sizeof(T) == GetSize());
	auto map = MabBuffer(*this);
	memcpy(map.template As<T>(), output.data(), output.size());
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
	assert(mBuffer.mMapRefCount >= 0);
	assert(mBuffer.mMapRefCount == 0 && mBuffer.);
	if (mBuffer.mMapRefCount == 0)
	{
		glBindBuffer(T::TargetValue, mBuffer.m_id);
		mBuffer.mCachedMappedPtr = glMapBuffer(T::TargetValue, accessHint);
	}
	++mBuffer.mMapRefCount;
}
template<class T>
MapBuffer<T>::~MapBuffer() noexcept
{
	assert(mBuffer.mMapRefCount > 0);
	assert(!mBuffer.mCachedMappedPtr.has_value());
	--mBuffer.mMapRefCount;
	if (mBuffer.mMapRefCount == 0)
	{
		if (mBuffer.mCachedMappedPtr.value() != nullptr)
		{
			glBindBuffer(T::TargetValue, mBuffer.m_id);
			glUnmapBuffer(T::TargetValue);
		}
		mBuffer.mCachedMappedPtr.reset();
	}
}

template<class T>
void* MapBuffer<T>::RawPointer() const noexcept
{
	return mBuffer.mCachedMappedPtr.value();
}
template<class T>
template<typename U>
U* MapBuffer<T>::As() const noexcept
{
	return static_cast<U*>(RawPointer());
}

// =====================================Convenience aliases definitions ============================
using ShaderStorageBuffer = BufferObject<GL_SHADER_STORAGE_BUFFER>;

// ========================================= Concept tests =========================================
static_assert(IsBufferObject<ShaderStorageBuffer>);
static_assert(!IsBufferObject<int>);

} // namespace open_gl
} // namespace core_gfx

#endif // BUFFER_OBJECT_H
