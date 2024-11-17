#include "Program.h"
namespace core_gfx::open_gl
{
Program::Program() { mProgId = glCreateProgram(); }

Program::~Program() { glDeleteProgram(mProgId); }

std::string Program::GetInfoLog() const
{
	GLint logsize;
	glGetProgramiv(mProgId, GL_INFO_LOG_LENGTH, &logsize);
	std::string log(logsize, 0);
	glGetProgramInfoLog(mProgId, logsize, nullptr, log.data());
	return log;
}

void Program::Use()
{
	glValidateProgram(mProgId);
	GLint status;
	glGetProgramiv(mProgId, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
		throw std::runtime_error(absl::StrFormat("Unable to use program\n%s", GetInfoLog()));
	glUseProgram(mProgId);
}

} // namespace core_gfx::open_gl
