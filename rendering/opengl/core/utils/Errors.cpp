#include "Errors.h"

#include <glad/glad.h>
#include <map>
#include <string>
#include <iostream>

namespace {


std::string ErrCodeName(GLint err)
{
	static const std::map<GLint, std::string> codeNames {
		{ GL_INVALID_ENUM, "GL_INVALID_ENUM"},
		{ GL_INVALID_VALUE, "GL_INVALID_VALUE"},
		{ GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
		{ GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
		{ GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
		{ GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
		{ GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"}
	};
	if(!codeNames.contains(err))
		return "UNDEFINED";
	return codeNames.at(err);
}

}
namespace core_gfx::open_gl::utils
{

bool CheckErrors()
{
	GLint err = glGetError();
	bool hasError = false;
	while(err != GL_NO_ERROR)
	{
		std::cout << "Opengl pipeline error: " << ErrCodeName(err) << std::endl;
		err = glGetError();
		hasError = true;
	}
	return hasError;
}
}
