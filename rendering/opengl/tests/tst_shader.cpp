
#define BOOST_TEST_MODULE OpenglShaderTest
#include <boost/test/unit_test.hpp>
#include <Shader.h>
#include <string>
#include <glad/glad.h>
#include <GL/osmesa.h>

using namespace core_gfx::open_gl;
class GLMesaTestSuite {
public:
	GLMesaTestSuite()
	{
		ctx = OSMesaCreateContextExt(GL_RGBA, 16, 0, 0, NULL);
		BOOST_REQUIRE(ctx);

			   // Create a buffer for rendering
		const int width = 800, height = 600;
		buffer.resize(width * height * 4, 0); // RGBA

		bool ok = OSMesaMakeCurrent(ctx, buffer.data(), GL_UNSIGNED_BYTE, width, height);
		BOOST_REQUIRE(ok);
		BOOST_REQUIRE(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(OSMesaGetProcAddress)));
	}
	~GLMesaTestSuite()
	{
		OSMesaDestroyContext(ctx);
	}
private:
	OSMesaContext ctx;
	std::vector<unsigned char> buffer;
};
BOOST_AUTO_TEST_SUITE (TestShader)

BOOST_FIXTURE_TEST_CASE(TestShaderCompilation, GLMesaTestSuite)
{

	VertexShader vshader(std::string{"#version 440 core\n void main() {}"});
	FragmentShader fshader(std::string{"#version 440 core\n void main(){}"});
	GeometryShader gshader(std::string{"#version 440 core\n void main(){}"});
	TessControlShader tcshader(std::string{"#version 440 core\n void main(){}"});
	TessEvaluationShader tevshader(std::string{"#version 440 core\n void main(){}"});
	ComputeShader cshader(std::string{"#version 440 core\n void main(){}"});
	try {
		vshader.Compile();
		fshader.Compile();
		gshader.Compile();
		tcshader.Compile();
		tevshader.Compile();
		cshader.Compile();

		std::cout << "Compile log: " << vshader.GetInfoLog() << std::endl;
	}
	catch(const std::exception& e)
	{
		BOOST_CHECK_MESSAGE(false, e.what());
	}

}

BOOST_FIXTURE_TEST_CASE(TestShaderFileCompilation, GLMesaTestSuite)
{
	VertexShader vshaderfile(std::filesystem::path("shaders/vert.glsl"));
	try{ vshaderfile.Compile(); } catch(const std::exception& e){BOOST_CHECK_MESSAGE(false, e.what());}

}


BOOST_AUTO_TEST_SUITE_END ()
