#ifndef lkasdksla
#define lkasdksla

#include "Ui.h"
#include <memory>
#include <glm/vec2.hpp>
namespace opengl::ui
{
namespace detail
{
class GlfwImpl;
}
class Glfw : public Ui
{
public:
	explicit Glfw(
		int versionMaajor = 4, int versionMinor = 4, int width = 1000, int height = 1000,
		std::function<void(int, int)>&& resizeCallback = [](int, int) {},
		std::function<void(const glm::ivec2& /*xy pos*/, const glm::ivec2& /*dx/dy offset*/)>&&
			cursorMoveCallback
		= [](const glm::ivec2&, const glm::ivec2&) {},
		std::function<void(double, double)> scrollCallback = [](double, double) {});
	~Glfw() override;
	void Run(std::function<bool()>&& functor) final;
	void Resize(int width, int height);
	void CursorMove(int x, int y);
	void Scroll(double xoffset, double yoffset);
	bool IsLeftMButtonPressed() const;
	bool IsRightMButtonPressed() const;
	bool IsShiftPressed() const;

private:
	std::unique_ptr<detail::GlfwImpl> impl;
};
} // namespace opengl::ui
#endif
