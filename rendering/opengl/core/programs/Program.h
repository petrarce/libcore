#ifndef PROGRAM_H
#define PROGRAM_H
#include "programs/Shader.h"
#include <glad/glad.h>
#include <concepts>

namespace core_gfx::open_gl
{

template<class T>
concept IsShader = std::derived_from<T, detail::ShaderBase>;

class Program
{
public:
	Program();

	~Program();

	std::string GetInfoLog() const;

	template<IsShader ShaderT, class... Args>
	void Setup(ShaderT& shader, Args&&... shaders)
	{
		SetupShaders(shader, shaders...);
		glLinkProgram(mProgId);

		GLint status;
		glGetProgramiv(mProgId, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
			throw std::runtime_error(absl::StrFormat("Failed to link program\n%s", GetInfoLog()));
	}

	void Use();
	GLint GetLocation(const std::string& name) const;

private:
	template<IsShader ShaderT>
	void SetupShaders(ShaderT& shader)
	{
		shader.Compile();
		glAttachShader(mProgId, shader.Id());
	}

	template<IsShader ShaderT, class... Args>
	void SetupShaders(ShaderT& shader, Args&&... shaders)
	{
		SetupShaders(shader);
		SetupShaders(shaders...);
	}
	GLint mProgId;
};

} // namespace core_gfx::open_gl
#endif // PROGRAM_H
