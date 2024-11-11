#include "Shader.h"

namespace core_gfx::open_gl
{

detail::ShaderBase::~ShaderBase()
{
	glDeleteShader(mShaderId);
}

void detail::ShaderBase::Compile()
{
	GLint status;
	glCompileShader(mShaderId);
	glGetShaderiv(mShaderId, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
		throw std::runtime_error(absl::StrFormat("Failed to compile shader\n%s", GetInfoLog()));
}

GLint detail::ShaderBase::Id() const {return mShaderId; }

}
