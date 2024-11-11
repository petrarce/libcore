#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <absl/strings/str_format.h>


namespace core_gfx::open_gl
{

namespace detail {
class ShaderBase
{
public:
	ShaderBase() = default;
	~ShaderBase();
	void Compile();
	GLint Id() const;

	std::string GetInfoLog() {
		GLint logsize;
		glGetShaderiv(mShaderId, GL_INFO_LOG_LENGTH, &logsize);
		std::string log(logsize, 0);
		glGetShaderInfoLog(mShaderId, logsize, nullptr, log.data());
		return log;
	}

protected:
	GLint mShaderId;
};
}

template<GLint SType>
class Shader : public detail::ShaderBase
{
public:
	Shader() = delete;
	explicit Shader(const ::std::string& shader)
		: ShaderBase()
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

}
#endif // SHADER_H
