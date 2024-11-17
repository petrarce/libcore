#ifndef VERTEXARRAYBUFFER_H
#define VERTEXARRAYBUFFER_H

#include <glad/glad.h>
#include <vector>
#include <utils/Errors.h>
#include <stdexcept>
#include <optional>
namespace core_gfx::open_gl
{

template<class T>
concept IsBufferType
	= std::is_same_v<T, float> || std::is_same_v<T, int8_t> || std::is_same_v<T, int16_t>
	  || std::is_same_v<T, int32_t> || std::is_same_v<T, uint8_t> || std::is_same_v<T, uint16_t>
	  || std::is_same_v<T, uint32_t>;
static_assert(IsBufferType<float>);
static_assert(IsBufferType<int>);
static_assert(!IsBufferType<std::optional<int> >);

class VertexArrayBuffer
{
public:
	explicit VertexArrayBuffer() = default;
	~VertexArrayBuffer();
	;

	void Use();
	void Unuse();

	template<class... Args, IsBufferType T>
	void Init(const std::vector<T>& buffer, int componentsCnt, Args&&... args)
	{
		Deinit();
		GLuint vao;
		// Generate vertex array and bind
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		try
		{
			mVBO = std::vector<GLuint>{};
			InitArrayBuffer(0, std::nullopt, buffer, componentsCnt, std::forward<Args>(args)...);
		}
		catch (const std::exception& e)
		{
			Deinit();
			throw std::runtime_error(e.what());
		}
		catch (...)
		{
			Deinit();
			throw std::runtime_error("Unknown error...");
		}

		mElements = buffer.size() / componentsCnt;
		mVAO = vao;
		glBindVertexArray(0);
	}
	void Deinit();
	const int Elements() const { return mElements.value_or(0); }

	std::vector<int> ListEnabledAttributes();

private:
	template<IsBufferType T>
	void InitArrayBuffer(const int& bufCnt, const std::optional<int>& expectedElements,
						 const std::vector<T>& buffer, const int& vertexElements)
	{
		if (buffer.size() % vertexElements != 0)
			throw std::runtime_error("The buffer is not aligned to the number of vertex elements");

		if (expectedElements.has_value()
			&& buffer.size() / vertexElements != expectedElements.value())
			throw std::runtime_error("The buffer is not aligned to other buffers");

		GLuint vbo;

		// Configure and load bufferts
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), buffer.data(), GL_STATIC_DRAW);

		// vertex attributes
		const GLint type = []()
		{
			if constexpr (std::is_same_v<T, float>)
				return GL_FLOAT;
			if constexpr (std::is_same_v<T, int8_t>)
				return GL_BYTE;
			if constexpr (std::is_same_v<T, uint8_t>)
				return GL_UNSIGNED_BYTE;
			if constexpr (std::is_same_v<T, int16_t>)
				return GL_SHORT;
			if constexpr (std::is_same_v<T, uint16_t>)
				return GL_UNSIGNED_SHORT;
			if constexpr (std::is_same_v<T, int32_t>)
				return GL_INT;
			if constexpr (std::is_same_v<T, uint32_t>)
				return GL_UNSIGNED_INT;
			else
				static_assert("Not supported type");
		}();
		glVertexAttribPointer(bufCnt, vertexElements, type, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(bufCnt);

		if (utils::CheckErrors())
		{
			Deinit();
			throw std::runtime_error("Failed to initialize the vertex array");
		}
		mVBO.value().push_back(vbo);
	}

	template<class T, class... Args>
	void InitArrayBuffer(const int& bufCnt, const std::optional<int>& expectedElements,
						 const std::vector<T>& buffer, const int& vertexElements, Args&&... args)
	{
		InitArrayBuffer(bufCnt, expectedElements, buffer, vertexElements);
		if constexpr (sizeof...(args) > 0)
			InitArrayBuffer(bufCnt + 1, expectedElements.value_or(buffer.size() / vertexElements),
							std::forward<Args>(args)...);
	}

	std::optional<GLuint> mVAO;
	std::optional<std::vector<GLuint> > mVBO;
	std::optional<int> mElements;
};

} // namespace core_gfx::open_gl
#endif // VERTEXARRAYBUFFER_H
