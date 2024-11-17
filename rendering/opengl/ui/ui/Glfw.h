#ifndef lkasdksla
#define lkasdksla

#include "Ui.h"
#include <memory>
namespace opengl::ui
{
namespace detail
{
class GlfwImpl;
}
class Glfw : public Ui
{
public:
	Glfw(
		int versionMaajor = 4, int versionMinor = 4, int width = 1000, int height = 1000,
		std::function<void(int, int)>&& resizeCallback = [](int, int) {});
	virtual ~Glfw();
	void Run(std::function<bool()>&& functor) final;
	void Resize(int width, int height);

private:
	std::unique_ptr<detail::GlfwImpl> impl;
};
} // namespace opengl::ui
#endif
