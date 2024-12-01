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

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	opengl::ui::Glfw* ptr = static_cast<opengl::ui::Glfw*>(glfwGetWindowUserPointer(window));

	ptr->Scroll(xoffset, yoffset);
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
	std::function<void(double, double)> scrollCallback;
	glm::ivec2 cursorPos;
};

} // namespace detail

Glfw::Glfw(int vmajor, int vminor, int width, int height,
		   std::function<void(int, int)>&& resizeCallback,
		   std::function<void(const glm::ivec2&, const glm::ivec2&)>&& cursorMoveCallback,
		   std::function<void(double, double)> scrollCallback)
	: Ui()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, vmajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, vminor);
	impl = std::make_unique<detail::GlfwImpl>(width, height, "GlfwWindow");
	impl->resizeCallback = resizeCallback;
	impl->cursorMoveCallback = cursorMoveCallback;
	impl->scrollCallback = scrollCallback;
	glfwMakeContextCurrent(impl->window.get());
	glfwSwapInterval(1);
	glfwSetWindowUserPointer(impl->window.get(), this);
	glfwSetFramebufferSizeCallback(impl->window.get(), FramebufferResizeCallback);
	glfwSetCursorPosCallback(impl->window.get(), CursorMoveCallback);
	glfwSetScrollCallback(impl->window.get(), ScrollCallback);
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

	const glm::vec2 dxdyf = dmove;
	if (IsLeftMButtonPressed())
	{

		if (IsShiftPressed())
			mDefaultCamera.Move(dxdyf.y / 50, -dxdyf.x / 50);
		else
			mDefaultCamera.Rotate(glm::vec3{ -static_cast<float>(dxdyf.x / 180.f * M_PI),
											 -static_cast<float>(dxdyf.y / 180.f * M_PI), 0 });
	}
}

void Glfw::Scroll(double xoffset, double yoffset)
{
	mDefaultCamera.MoveTowardsTarget(-yoffset);

	impl->scrollCallback(xoffset, yoffset);
}

bool Glfw::IsLeftMButtonPressed() const
{
	return GLFW_PRESS == glfwGetMouseButton(impl->window.get(), GLFW_MOUSE_BUTTON_LEFT);
}

bool Glfw::IsRightMButtonPressed() const
{
	return GLFW_PRESS == glfwGetMouseButton(impl->window.get(), GLFW_MOUSE_BUTTON_RIGHT);
}

bool Glfw::IsShiftPressed() const
{
	return glfwGetKey(impl->window.get(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
		   || glfwGetKey(impl->window.get(), GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
}

} // namespace opengl::ui
