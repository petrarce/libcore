#include "TexturedVertexArrayBuffer.h"
#include <SOIL2.h>
namespace core_gfx::open_gl
{

void TexturedVertexArrayBuffer::LoadTexture(const std::filesystem::path& filePath)
{
	GLuint tex = SOIL_load_OGL_texture(filePath.c_str(), 0, 0,
									   SOIL_FLAG_MIPMAPS | SOIL_FLAG_TEXTURE_REPEATS
										   | SOIL_FLAG_MULTIPLY_ALPHA);
	mTextureObjects->push_back(tex);
}

void TexturedVertexArrayBuffer::Use()
{
	VertexArrayBuffer::Use();
	if (!mTextureObjects.has_value())
		return;
	for (int i = 0; i < mTextureObjects->size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, (*mTextureObjects)[i]);
	}
}

void TexturedVertexArrayBuffer::Deinit()
{
	if (mTextureObjects.has_value())
		glDeleteTextures(mTextureObjects->size(), mTextureObjects->data());

	VertexArrayBuffer::Deinit();
}

} // namespace core_gfx::open_gl
