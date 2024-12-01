#ifndef TEXTUREDVERTEXARRAYBUFFER_H
#define TEXTUREDVERTEXARRAYBUFFER_H
#include "VertexArrayBuffer.h"
#include <filesystem>

namespace core_gfx::open_gl
{
class TexturedVertexArrayBuffer : public VertexArrayBuffer
{
public:
	explicit TexturedVertexArrayBuffer() = default;
	template<class... Args>
	void InitTextures(const std::filesystem::path& tex, Args&&... args)
	{
		if (mTextureObjects.has_value())
			throw std::runtime_error("TexturedVertexArrayBuffer is not empty");
		mTextureObjects = std::vector<GLuint>{};
		LoadTexture(tex, std::forward<Args>(args)...);
	}

	void Use() override;
	void Deinit() override;

private:
	template<class... Args>
	void LoadTextures(const std::filesystem::path& tex, Args&&... args)
	{
		LoadTextures(tex);
		if constexpr (sizeof...(args) > 0)
			LoadTextures(std::forward<Args>(args)...);
	}
	void LoadTexture(const std::filesystem::path& filePath);

	std::optional<std::vector<GLuint> > mTextureObjects;
};
} // namespace core_gfx::open_gl
#endif // TEXTUREDVERTEXARRAYBUFFER_H
