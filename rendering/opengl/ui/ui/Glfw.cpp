#include "Glfw.h"
#include "GLFW/glfw3.h"
#include <memory>

namespace
{
void FramebufferResizeCallback(GLFWwindow* win, int width, int height)
{
	opengl::ui::Glfw* ptr = static_cast<opengl::ui::Glfw*>(glfwGetWindowUserPointer(win));
	ptr->Resize(width, height);
}
} // namespace

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
	std::function<void(int, int)> resizeCallback;
};

} // namespace detail

Glfw::Glfw(int vmajor, int vminor, int width, int height,
		   std::function<void(int, int)>&& resizeCallback)
	: Ui()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vmajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vminor);
	impl = std::make_unique<detail::GlfwImpl>(width, height, "GlfwWindow");
	impl->resizeCallback = resizeCallback;
	glfwMakeContextCurrent(impl->window.get());
	glfwSwapInterval(1);
	glfwSetWindowUserPointer(impl->window.get(), this);
	glfwSetFramebufferSizeCallback(impl->window.get(), FramebufferResizeCallback);
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

void Glfw::Resize(int width, int height) { impl->resizeCallback(width, height); }

} // namespace opengl::ui
