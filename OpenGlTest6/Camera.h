#ifndef CAMERA_HANDLE
#define CAMERA_HANDLE
#include <glm/glm.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

class Camera {
	public:
		Camera();
		glm::mat4 getWorldToViewMatrix() const;
		glm::mat4 projectionMatrix;
	private:
		glm::vec3 position;
		glm::vec3 viewDirection;
		const glm::vec3 up;
};

Camera::Camera() 
	: position(0.0f, 0.0f, 0.0f),
	viewDirection(1.0f, 0.0f, 0.0f),
	up(0.0f, 1.0f, 0.0f)
{
	projectionMatrix = glm::mat4(
		vec4(1, 0, 0, 2),
		vec4(0, 1, 0, -5),
		vec4(0, 0, 1, 9),
		vec4(0, 0, 0, 1)
	);
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, up);
}

#endif