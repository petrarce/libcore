#include "Glfw.h"
#include "GLFW/glfw3.h"
#include <memory>
namespace opengl::ui
{

namespace detail
{
class GlfwImpl
{

public:
	GlfwImpl(int width, int height, const std::string& name)
		: window(glfwCreateWindow(width, height, name.c_str(), NULL, NULL))
	{
	}
	struct GLFWwindowDeleter
	{
		void operator()(GLFWwindow* ptr) { glfwDestroyWindow(ptr); }
	};
	using GLFWwindowUniquePtr = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>;
	GLFWwindowUniquePtr window;
};

} // namespace detail

Glfw::Glfw(int vmajor, int vminor, int width, int height)
	: Ui()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vmajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vminor);
	impl = std::make_unique<detail::GlfwImpl>(width, height, "GlfwWindow");
	glfwMakeContextCurrent(impl->window.get());
	glfwSwapInterval(1);
}

Glfw::~Glfw()
{
	impl.reset();
	glfwTerminate();
}

void Glfw::Run(std::function<bool()>&& functor)
{
	while (!glfwWindowShouldClose(impl->window.get()) && !functor())
	{
		glfwSwapBuffers(impl->window.get());
		glfwPollEvents();
	}
}

} // namespace opengl::ui
