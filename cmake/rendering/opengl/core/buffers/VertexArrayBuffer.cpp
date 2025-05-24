#include "VertexArrayBuffer.h"

namespace core_gfx::open_gl
{

VertexArrayBuffer::~VertexArrayBuffer() { Deinit(); }

void VertexArrayBuffer::Use() { glBindVertexArray(mVAO.value()); }

void VertexArrayBuffer::Unuse() { glBindVertexArray(0); }

void VertexArrayBuffer::Deinit()
{
	if (mVBO.has_value())
		glDeleteBuffers(mVBO.value().size(), mVBO.value().data());

	if (mVAO.has_value())
		glDeleteVertexArrays(1, &mVAO.value());

	if (mEBO.has_value())
		glDeleteBuffers(1, &mEBO.value());

	mVBO.reset();
	mVAO.reset();
	mEBO.reset();
	mElements.reset();
}

std::vector<GLint> VertexArrayBuffer::ListEnabledAttributes()
{
	if (!mVAO.has_value())
		return std::vector<int>{};

	std::vector<int> enabledAttributes;
	Use();
	const int maxAttributes = [&]()
	{
		GLint maxAttribs = 0;
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
		return maxAttribs;
	}();

	for (GLint attrib = 0; attrib < maxAttributes; attrib++)
	{
		GLint enabled;
		glGetVertexAttribiv(attrib, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
		if (!enabled)
			continue;
		enabledAttributes.push_back(attrib);
	}
	return enabledAttributes;
}

} // namespace core_gfx::open_gl
