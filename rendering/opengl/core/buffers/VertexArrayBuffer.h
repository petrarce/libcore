#ifndef VERTEXARRAYBUFFER_H
#define VERTEXARRAYBUFFER_H

#include <glad/glad.h>
#include <vector>
#include <concepts>
#include <utils/Errors.h>
namespace core_gfx::open_gl
{

template<class T>
concept IsBufferType = std::floating_point<T>;

template<IsBufferType T>
class VertexArrayBuffer
{
public:
	explicit VertexArrayBuffer(const std::vector<T>& buffer, int vertexElements = 3);

	void Use()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindVertexArray(mVAO);
		const GLint type = []()
		{
			if constexpr (std::floating_point<T>)
				return GL_FLOAT;
			else
				static_assert("Not supported type");
		}();

		glVertexAttribPointer(0, mElements, type, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
	}
	~VertexArrayBuffer();

private:
	GLuint mVAO;
	GLuint mVBO;
	int mElements;
};

template<IsBufferType T>
inline VertexArrayBuffer<T>::VertexArrayBuffer(const std::vector<T>& buffer, int vertexElements)
	: mElements(vertexElements)
{
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * sizeof(T), buffer.data(), GL_STATIC_DRAW);
	utils::CheckErrors();
}

template<IsBufferType T>
inline VertexArrayBuffer<T>::~VertexArrayBuffer()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
}

} // namespace core_gfx::open_gl
#endif // VERTEXARRAYBUFFER_H
