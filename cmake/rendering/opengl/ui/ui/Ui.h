#ifndef lkajsdkjasd
#define lkajsdkjasd
#include <functional>
#include <utils/Camera.h>
namespace opengl::ui
{

class Ui
{
public:
	Ui() = default;
	virtual ~Ui() = default;

	// make non-copyable
	Ui(const Ui& other) = delete;
	Ui(Ui&&) = delete;
	Ui& operator=(const Ui&) = delete;
	Ui& operator=(Ui&&) = delete;

	virtual void Run(std::function<bool()>&& functor) = 0;
	glm::mat4 DefaultCamView() { return mDefaultCamera.ViewMatrix(); }

protected:
	rendering::utils::Camera mDefaultCamera;
};

} // namespace opengl::ui

#endif
