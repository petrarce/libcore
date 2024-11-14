#ifndef lkajsdkjasd
#define lkajsdkjasd
#include <functional>
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
};

} // namespace opengl::ui

#endif
