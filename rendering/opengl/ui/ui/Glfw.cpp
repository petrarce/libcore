#include "Glfw.h"
#include "GLFW/glfw3.h"
#include <memory>
#include <glm/vec2.hpp>
namespace
{
void FramebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	opengl::ui::Glfw* ptr = static_cast<opengl::ui::Glfw*>(glfwGetWindowUserPointer(window));
	ptr->Resize(width, height);
}

void CursorMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	opengl::ui::Glfw* ptr = static_cast<opengl::ui::Glfw*>(glfwGetWindowUserPointer(window));
	ptr->CursorMove(xpos, ypos);
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
	std::function<void(const glm::ivec2&, const glm::ivec2&)> cursorMoveCallback;
	glm::ivec2 cursorPos;
};

} // namespace detail

Glfw::Glfw(int vmajor, int vminor, int width, int height,
		   std::function<void(int, int)>&& resizeCallback,
		   std::function<void(const glm::ivec2&, const glm::ivec2&)>&& cursorMoveCallback)
	: Ui()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vmajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vminor);
	impl = std::make_unique<detail::GlfwImpl>(width, height, "GlfwWindow");
	impl->resizeCallback = resizeCallback;
	impl->cursorMoveCallback = cursorMoveCallback;
	glfwMakeContextCurrent(impl->window.get());
	glfwSwapInterval(1);
	glfwSetWindowUserPointer(impl->window.get(), this);
	glfwSetFramebufferSizeCallback(impl->window.get(), FramebufferResizeCallback);
	glfwSetCursorPosCallback(impl->window.get(), CursorMoveCallback);
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

void Glfw::CursorMove(int x, int y)
{
	glm::ivec2 dmove = glm::ivec2{ x, y } - impl->cursorPos;
	impl->cursorMoveCallback({ x, y }, dmove);
	impl->cursorPos += dmove;
}

bool Glfw::IsLeftMButtonPressed()
{
	return GLFW_PRESS == glfwGetMouseButton(impl->window.get(), GLFW_MOUSE_BUTTON_LEFT);
}

bool Glfw::IsRightMButtonPressed()
{
	return GLFW_PRESS == glfwGetMouseButton(impl->window.get(), GLFW_MOUSE_BUTTON_RIGHT);
}

} // namespace opengl::ui
