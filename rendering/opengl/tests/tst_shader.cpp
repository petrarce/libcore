
#define BOOST_TEST_DYN_LINK
#include "GLMesaTestFixture.h"

#include <boost/test/unit_test.hpp>
#include <Shader.h>
#include <string>
#include <glad/glad.h>

using namespace core_gfx::open_gl;

BOOST_AUTO_TEST_SUITE(TestShader)

BOOST_FIXTURE_TEST_CASE(TestShaderCompilation, GLMesaTestFixture)
{

	VertexShader vshader(std::string{ "#version 440 core\n void main() {}" });
	FragmentShader fshader(std::string{ "#version 440 core\n void main(){}" });
	GeometryShader gshader(std::string{ "#version 440 core\n void main(){}" });
	TessControlShader tcshader(std::string{ "#version 440 core\n void main(){}" });
	TessEvaluationShader tevshader(std::string{ "#version 440 core\n void main(){}" });
	ComputeShader cshader(std::string{ "#version 440 core\n void main(){}" });
	try
	{
		vshader.Compile();
		fshader.Compile();
		gshader.Compile();
		tcshader.Compile();
		tevshader.Compile();
		cshader.Compile();

		std::cout << "Compile log: " << vshader.GetInfoLog() << std::endl;
	}
	catch (const std::exception& e)
	{
		BOOST_CHECK_MESSAGE(false, e.what());
	}
}

BOOST_FIXTURE_TEST_CASE(TestShaderFileCompilation, GLMesaTestFixture)
{
	VertexShader vshaderfile(std::filesystem::path("shaders/vert.glsl"));
	try
	{
		vshaderfile.Compile();
	}
	catch (const std::exception& e)
	{
		BOOST_CHECK_MESSAGE(false, e.what());
	}
}

BOOST_AUTO_TEST_SUITE_END()
