#ifndef lksjdlfkjsdlfkjf
#define lksjdlfkjsdlfkjf
#include <cstdint>
#include <glm/mat4x4.hpp>
#include <memory>

namespace rendering::utils
{
namespace detail
{
class CameraImpl;
}
class Camera
{
public:
	enum class CameraMode : uint8_t
	{
		Orbit
	};
	explicit Camera(CameraMode type = Camera::CameraMode::Orbit);
	~Camera();

	glm::mat4 ViewMatrix();
	void Rotate(const glm::vec3& pyrRad);
	void LookAt(const glm::vec3& pos);
	void Move(float fwd, float side);
	void MoveTowardsTarget(float in);

private:
	std::unique_ptr<detail::CameraImpl> impl;
};
} // namespace rendering::utils

#endif
