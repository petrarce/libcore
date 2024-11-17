#include <ui/Glfw.h>
#include <programs/Program.h>
#include <buffers/VertexArrayBuffer.h>

using namespace opengl::ui;
using namespace core_gfx::open_gl;

int main()
{
	Glfw glfwWindow(4, 4, 1000, 1000,
					[](int width, int height) { glViewport(0, 0, width, height); });
	gladLoadGL();

	VertexShader vs(std::filesystem::path("simple_uniform_test/vert.glsl"));
	FragmentShader fs(std::filesystem::path("simple_uniform_test/frag.glsl"));
	Program prog;
	prog.Setup(fs, vs);
	prog.Use();

	const auto colorLoc = prog.GetLocation("color");
	VertexArrayBuffer buf;
	buf.Init(std::vector<float>{ -0.5, -0.5, 0.5, 0.5, 0.5, -0.5 }, 2);

	float color = 0;
	glDepthFunc(GL_ALWAYS);

	glfwWindow.Run(
		[&]()
		{
			glClearColor(0, 0, 0, 1);
			glClearDepth(1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// color = (color + 0.1) - std::floor(color) + 0.5;
			color = (color + 0.01) - static_cast<int>(color + 0.01);
			glUniform1f(colorLoc, color);

			buf.Use();
			glDrawArrays(GL_TRIANGLES, 0, buf.Elements());
			buf.Unuse();
			return false;
		});
	return 0;
}
