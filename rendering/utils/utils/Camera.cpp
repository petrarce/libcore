#include "Camera.h"
#include <camera/camera.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace
{
glm::vec3 toGlm(const CameraVec3& v) { return { v.x, v.y, v.z }; }
} // namespace
namespace rendering::utils
{
namespace detail
{

const glm::mat3 camWordMat
	= { toGlm(CAMERA_WORLD_FORWARD), toGlm(CAMERA_WORLD_RIGHT), toGlm(CAMERA_WORLD_UP) };

class CameraImpl
{
public:
	::Camera camera;
};
} // namespace detail

Camera::Camera(CameraMode type)
	: impl(new detail::CameraImpl{})
{
	impl->camera = camera_init();
	switch (type)
	{
	case CameraMode::Orbit:
	default:
		impl->camera.mode = CAMERA_MODE_FIRST_PERSON;
		impl->camera.minPitch = -M_PI_2;
		impl->camera.maxPitch = M_PI_2;
		impl->camera.target_distance = 10;
	}
}

Camera::~Camera() { }

glm::mat4 Camera::ViewMatrix()
{
	glm::mat4 view;
	camera_view_matrix(&impl->camera, glm::value_ptr(view));
	return view;
}

void Camera::Rotate(const glm::vec3& pyrRad)
{
	camera_rotate(&impl->camera, CameraVec3{ pyrRad.y, pyrRad.x, pyrRad.z });
}

void Camera::LookAt(const glm::vec3& pos)
{
	impl->camera.target_position = CameraVec3{ pos.x, pos.y, pos.z };
}

void Camera::Move(float fwd, float right) { camera_move(&impl->camera, { fwd, 0, right }); }

void Camera::MoveTowardsTarget(float in)
{
	float interpolant = (std::min(101.f, std::max(1.f, impl->camera.target_distance)) - 1) / 100.f;
	interpolant = std::sqrt(interpolant);
	impl->camera.target_distance
		= std::min(100.f, std::max(1.f, impl->camera.target_distance + in * interpolant));
}

} // namespace rendering::utils
