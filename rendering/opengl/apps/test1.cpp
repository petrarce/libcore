#include <ui/Glfw.h>
#include <glad/glad.h>
using namespace opengl::ui;

int main()
{
	Glfw glfw;
	gladLoadGL();

	glfw.Run([](){
		glClearColor(0, 1, 0, 1);
		glClearDepth(1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		return false;
	});
}
