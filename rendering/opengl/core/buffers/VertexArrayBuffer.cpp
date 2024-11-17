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
		glDeleteVertexArrays(1, &(mVAO.value()));
	mVBO.reset();
	mVAO.reset();
}

} // namespace core_gfx::open_gl
