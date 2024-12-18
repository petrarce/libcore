#include <ui/Glfw.h>
#include <programs/Program.h>
#include <buffers/VertexArrayBuffer.h>
#include <buffers/TexturedVertexArrayBuffer.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <cmath>
#include <utils/Camera.h>

using namespace opengl::ui;
using namespace core_gfx::open_gl;
using namespace rendering::utils;

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
	Glfw glfwWindow(4, 4, 1000, 1000,
					[](int width, int height) { glViewport(0, 0, width, height); });
	gladLoadGL();

	VertexShader vs(std::filesystem::path("simple_uniform_test/vert.glsl"));
	VertexShader vsDiff(std::filesystem::path("simple_uniform_test/vert_diff.glsl"));
	FragmentShader fs(std::filesystem::path("simple_uniform_test/frag.glsl"));
	FragmentShader fsDiff(std::filesystem::path("simple_uniform_test/frag_diff.glsl"));
	Program prog, progDiffuse;
	prog.Setup(fs, vs);
	progDiffuse.Setup(vsDiff, fsDiff);

	VertexArrayBuffer buf;
	// clang-format off
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
	VertexArrayBuffer piramid;
	piramid.InitElements(
		std::vector<uint32_t>{
			// Base
			0, 1, 2,
			2, 3, 0,
			// Sides
			0, 1, 4,
			1, 2, 4,
			2, 3, 4,
			3, 0, 4
		},
		std::vector<float>{
			-0.5f, 0.0f, 5 + -0.5f,  // Bottom-left
			0.5f,  0.0f, 5 + -0.5f,  // Bottom-right
			0.5f,  0.0f, 5 +  0.5f,  // Top-right
			-0.5f, 0.0f, 5 +  0.5f,  // Top-left
			0.0f,  2.0f, 5 +  0.0f   // Apex
		}, 3);

	TexturedVertexArrayBuffer texturedCube;
	texturedCube.InitElements(
		std::vector<uint32_t>{
			// Front face
			0, 1, 2,  2, 3, 0,
			// Back face
			4, 5, 6,  6, 7, 4,
			// Left face
			8, 9, 10, 10, 11, 8,
			// Right face
			12, 13, 14, 14, 15, 12,
			// Top face
			16, 17, 18, 18, 19, 16,
			// Bottom face
			20, 21, 22, 22, 23, 20
		},

		std::vector<float>{
			// Front face
			4.5f,  0.5f, 4.5f,  // Bottom-left
			5.5f,  0.5f, 4.5f,  // Bottom-right
			5.5f,  0.5f, 5.5f,  // Top-right
			4.5f,  0.5f, 5.5f,  // Top-left

			// Back face
			4.5f, -0.5f, 4.5f,  // Bottom-left
			5.5f, -0.5f, 4.5f,  // Bottom-right
			5.5f, -0.5f, 5.5f,  // Top-right
			4.5f, -0.5f, 5.5f,  // Top-left

			// Left face
			4.5f, -0.5f, 4.5f,  // Bottom-left
			4.5f,  0.5f, 4.5f,  // Bottom-right
			4.5f,  0.5f, 5.5f,  // Top-right
			4.5f, -0.5f, 5.5f,  // Top-left

			// Right face
			5.5f, -0.5f, 4.5f,  // Bottom-left
			5.5f,  0.5f, 4.5f,  // Bottom-right
			5.5f,  0.5f, 5.5f,  // Top-right
			5.5f, -0.5f, 5.5f,  // Top-left

			// Top face
			4.5f, -0.5f, 5.5f,  // Bottom-left
			5.5f, -0.5f, 5.5f,  // Bottom-right
			5.5f,  0.5f, 5.5f,  // Top-right
			4.5f,  0.5f, 5.5f,  // Top-left

			// Bottom fa ce
			4.5f, -0.5f, 4.5f,  // Bottom-left
			5.5f, -0.5f, 4.5f,  // Bottom-right
			5.5f,  0.5f, 4.5f,  // Top-right
			4.5f,  0.5f, 4.5f,// Top-left
		} , 3,
		std::vector<float>{
			// Front face
			0.0f, 0.0f,  // Bottom-left
			1.0f, 0.0f,  // Bottom-right
			1.0f, 1.0f,  // Top-right
			0.0f, 1.0f,  // Top-left

			// Back face
			0.0f, 0.0f,  // Bottom-left
			1.0f, 0.0f,  // Bottom-right
			1.0f, 1.0f,  // Top-right
			0.0f, 1.0f,  // Top-left

			// Left face
			0.0f, 0.0f,  // Bottom-left
			1.0f, 0.0f,  // Bottom-right
			1.0f, 1.0f,  // Top-right
			0.0f, 1.0f,  // Top-left

			// Right face
			0.0f, 0.0f,  // Bottom-left
			1.0f, 0.0f,  // Bottom-right
			1.0f, 1.0f,  // Top-right
			0.0f, 1.0f,  // Top-left

			// Top face
			0.0f, 0.0f,  // Bottom-left
			1.0f, 0.0f,  // Bottom-right
			1.0f, 1.0f,  // Top-right
			0.0f, 1.0f,  // Top-left

			// Bottom face
			0.0f, 0.0f,  // Bottom-left
			1.0f, 0.0f,  // Bottom-right
			1.0f, 1.0f,  // Top-right
			0.0f, 1.0f   // Top-left
		}, 2
	);
	texturedCube.InitTextures(std::filesystem::path("simple_uniform_test/bricks.jpg"));
	// clang-format on

	auto drawBuf = [](VertexArrayBuffer& buf)
	{
		buf.Use();
		if (!buf.HasIndices())
			glDrawArrays(GL_TRIANGLES, 0, buf.Elements());
		else
			glDrawElements(GL_TRIANGLES, buf.Elements(), GL_UNSIGNED_INT, 0);
		buf.Unuse();
	};
	float color = 0;
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

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
			glm::mat4 view = glfwWindow.DefaultCamView();
			glm::mat4 persp
				= glm::perspective<float>(M_PI_4, viewportSize.x / viewportSize.y, 0.01, 100.);
			const auto vpmat = persp * view;
			color = (color + 0.01) - static_cast<int>(color + 0.01);
			prog.Use();
			glUniform1f(prog.GetLocation("color"), color);
			glUniformMatrix4fv(prog.GetLocation("vpmat"), 1, false, glm::value_ptr(vpmat));
			drawBuf(buf);
			drawBuf(piramid);
			progDiffuse.Use();
			glUniformMatrix4fv(progDiffuse.GetLocation("vpmat"), 1, false, glm::value_ptr(vpmat));
			drawBuf(texturedCube);
			return false;
		});
	return 0;
}
