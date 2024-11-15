#include <ui/Glfw.h>
#include <glad/glad.h>
#include <Program.h>
#include <buffers/VertexArrayBuffer.h>
using namespace opengl::ui;
using namespace core_gfx::open_gl;

int main()
{
	Glfw glfw;
	gladLoadGL();

	Program prog;
	VertexShader vshader(std::filesystem::path("test2/vert.glsl"));
	FragmentShader fshader(std::filesystem::path("test2/frag.glsl"));
	prog.Setup(vshader, fshader);
	prog.Use();

	VertexArrayBuffer buf(std::vector<float>{ 0, 0, 1, 1, 1, 0 }, 2);
	buf.Use();
	glDepthFunc(GL_ALWAYS);

	glfw.Run(
		[&]()
		{
			glClearColor(0, 0, 0, 1);
			glClearDepth(1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			buf.Use();
			glDrawArrays(GL_TRIANGLES, 0, 3);
			return false;
		});
}
