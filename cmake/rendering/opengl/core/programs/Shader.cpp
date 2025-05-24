#include "Shader.h"
#include <map>
namespace
{

const std::string& ShaderName(GLint shader)
{
	static std::string underinedShader = "Undefined Shader";
	static std::map<GLint, std::string> snameMap = {
		{ GL_VERTEX_SHADER, "GL_VERTEX_SHADER" },
		{ GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER" },
		{ GL_GEOMETRY_SHADER, "GL_GEOMETRY_SHADER" },
		{ GL_TESS_CONTROL_SHADER, "GL_TESS_CONTROL_SHADER" },
		{ GL_TESS_EVALUATION_SHADER, "GL_TESS_EVALUATION_SHADER" },
		{ GL_FRAGMENT_SHADER, "GL_FRAGMENT_SHADER" },
		{ GL_COMPUTE_SHADER, "GL_COMPUTE_SHADER" },
	};
	return snameMap.contains(shader) ? snameMap.at(shader) : underinedShader;
}
} // namespace
namespace core_gfx::open_gl
{

detail::ShaderBase::~ShaderBase() { glDeleteShader(mShaderId); }

void detail::ShaderBase::Compile()
{
	GLint status;
	glCompileShader(mShaderId);
	glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
		throw std::runtime_error(absl::StrFormat("Failed to compile shader\n%s", GetInfoLog()));
}

GLint detail::ShaderBase::Id() const { return mShaderId; }

} // namespace core_gfx::open_gl
