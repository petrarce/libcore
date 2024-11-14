#ifndef PROGRAM_H
#define PROGRAM_H
#include "Shader.h"
#include <glad/glad.h>
#include <concepts>
#include <optional>

namespace core_gfx::open_gl
{

template<class T>
concept IsShader = std::derived_from<T, detail::ShaderBase>;

class Program
{
public:
	Program() { mProgId = glCreateProgram(); }

	~Program() { glDeleteProgram(mProgId); }

	std::string GetInfoLog() const
	{
		GLint logsize;
		glGetProgramiv(mProgId, GL_INFO_LOG_LENGTH, &logsize);
		std::string log(logsize, 0);
		glGetProgramInfoLog(mProgId, logsize, nullptr, log.data());
		return log;
	}

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

	void Use()
	{
		glValidateProgram(mProgId);
		GLint status;
		glGetProgramiv(mProgId, GL_VALIDATE_STATUS, &status);
		if (status == GL_FALSE)
			throw std::runtime_error(absl::StrFormat("Unable to use program\n%s", GetInfoLog()));
		glUseProgram(mProgId);
	}

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
