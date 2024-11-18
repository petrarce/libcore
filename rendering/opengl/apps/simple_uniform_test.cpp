#include <ui/Glfw.h>
#include <programs/Program.h>
#include <buffers/VertexArrayBuffer.h>
#include <camera/camera.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <cmath>
using namespace opengl::ui;
using namespace core_gfx::open_gl;

namespace glm
{
template<int N, class T, glm::qualifier Q>
std::ostream& operator<<(std::ostream& s, const glm::vec<N, T, Q>& vec)
{
	s << "[";
	for (int i = 0; i < N; i++)
		s << vec[i] << ",";
	return s << "]";
}

} // namespace glm

int main()
{
	auto camera = camera_init();
	camera.mode = CAMERA_MODE_THIRD_PERSON;
	camera.minPitch = -M_PI_4;
	camera.maxPitch = M_PI_4;
	camera.target_distance = 5;
	camera.target_position = CameraVec3{ 0, 0, 0 };

	Glfw glfwWindow(
		4, 4, 1000, 1000, [](int width, int height) { glViewport(0, 0, width, height); },
		[&](const glm::ivec2& xy, const glm::ivec2& dxdy)
		{
			if (!glfwWindow.IsLeftMButtonPressed())
				return;

			glm::vec2 dxdyf = dxdy;

			const auto rotation = CameraVec3{ -static_cast<float>(dxdyf.y / 180.f * M_PI),
											  -static_cast<float>(dxdyf.x / 180.f * M_PI), 0 };
			camera_rotate(&camera, rotation);
		});
	gladLoadGL();

	VertexShader vs(std::filesystem::path("simple_uniform_test/vert.glsl"));
	FragmentShader fs(std::filesystem::path("simple_uniform_test/frag.glsl"));
	Program prog;
	prog.Setup(fs, vs);
	prog.Use();

	const auto colorLoc = prog.GetLocation("color");
	const auto vpmatLoc = prog.GetLocation("vpmat");

	VertexArrayBuffer buf;
	buf.Init(
		std::vector<float>{
			// Front face
			-1.0f, -1.0f, 1.0f,										 // Triangle 1
			1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, // Triangle 2
			1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f,

			// Back face
			-1.0f, -1.0f, -1.0f,										// Triangle 1
			-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, // Triangle 2
			1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f,

			// Left face
			-1.0f, -1.0f, -1.0f,										// Triangle 1
			-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f, // Triangle 2
			-1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f,

			// Right face
			1.0f, -1.0f, -1.0f,										 // Triangle 1
			1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, // Triangle 2
			1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,

			// Top face
			-1.0f, 1.0f, -1.0f,										 // Triangle 1
			-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f, // Triangle 2
			1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,

			// Bottom face
			-1.0f, -1.0f, -1.0f,										// Triangle 1
			1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, // Triangle 2
			1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f },
		3);

	float color = 0;
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_CULL_FACE);

	glfwWindow.Run(
		[&]()
		{
			glClearColor(0, 0, 0, 1);
			glClearDepth(1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			GLint viewport[4];
			glGetIntegerv(GL_VIEWPORT, viewport);
			glm::vec2 viewportPos{ viewport[0], viewport[1] };
			glm::vec2 viewportSize{ viewport[2], viewport[3] };
			glm::mat4 view, persp;

			camera_view_matrix(&camera, glm::value_ptr(view));
			persp = glm::perspective<float>(M_PI_4, viewportSize.x / viewportSize.y, 0.01, 1000.);
			const auto vpmat = persp * view;
			color = (color + 0.01) - static_cast<int>(color + 0.01);
			glUniform1f(colorLoc, color);
			glUniformMatrix4fv(vpmatLoc, 1, false, glm::value_ptr(vpmat));

			buf.Use();
			glDrawArrays(GL_TRIANGLES, 0, buf.Elements());
			buf.Unuse();
			return false;
		});
	return 0;
}
