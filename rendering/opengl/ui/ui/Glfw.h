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
	Glfw(int versionMaajor = 4, int versionMinor = 4, int width = 1000, int height = 1000);
	virtual ~Glfw();
	void Run(std::function<bool()>&& functor) final;

private:
	std::unique_ptr<detail::GlfwImpl> impl;
};
} // namespace opengl::ui
#endif
