#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <absl/strings/str_format.h>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace core_gfx::open_gl
{

namespace detail
{
class ShaderBase
{
public:
	ShaderBase() = default;
	~ShaderBase();
	void Compile();
	GLint Id() const;

	std::string GetInfoLog()
	{
		GLint logsize;
		glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &logsize);
		std::string log(logsize, 0);
		glGetShaderInfoLog(mShaderId, logsize, nullptr, log.data());
		return log;
	}

protected:
	GLint mShaderId;
};
} // namespace detail

template<GLint SType>
class Shader : public detail::ShaderBase
{
public:
	Shader() = delete;

	explicit Shader(const ::std::string& shader)
		: ShaderBase()
	{
		Init(shader);
	}

	explicit Shader(const std::filesystem::path& shaderPath)
	{
		std::ifstream file(shaderPath);
		std::string shader;
		if (file.is_open())
		{
			std::stringstream sstream;
			sstream << file.rdbuf();
			shader = sstream.str();
		}
		else
			std::cout << "Failed to open file:" << shaderPath << std::endl;
		Init(shader);
	}

private:
	void Init(const ::std::string& shader)
	{
		mShaderId = glCreateShader(SType);
		const char* shader_str = shader.c_str();
		glShaderSource(mShaderId, 1, &shader_str, nullptr);
	}
};

using VertexShader = Shader<GL_VERTEX_SHADER>;
using TessControlShader = Shader<GL_TESS_CONTROL_SHADER>;
using TessEvaluationShader = Shader<GL_TESS_EVALUATION_SHADER>;
using GeometryShader = Shader<GL_GEOMETRY_SHADER>;
using FragmentShader = Shader<GL_FRAGMENT_SHADER>;
using ComputeShader = Shader<GL_COMPUTE_SHADER>;

} // namespace core_gfx::open_gl
#endif // SHADER_H
