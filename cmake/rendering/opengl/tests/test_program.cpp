
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "GLMesaTestFixture.h"
#include <programs/Program.h>
#include <programs/Shader.h>
#include <utils/Errors.h>

using namespace core_gfx::open_gl;

BOOST_AUTO_TEST_SUITE(TestProgram)

BOOST_FIXTURE_TEST_CASE(TestProgramConstruction, GLMesaTestFixture)
{
	Program prog;
	VertexShader vshader(std::filesystem::path("shaders/vert.glsl"));
	FragmentShader fshader(std::filesystem::path("shaders/frag.glsl"));

	prog.Setup(vshader, fshader);
	prog.Use();
	BOOST_CHECK(utils::CheckErrors() == false);
}

BOOST_AUTO_TEST_SUITE_END()
