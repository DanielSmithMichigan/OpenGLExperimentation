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
#include <GLFW/glfw3.h>

class Camera {
	public:
		Camera();
		glm::mat4 getWorldToViewMatrix() const;
		glm::mat4 projectionMatrix;
		void handleKeypress(int keypress);
	private:
		glm::vec3 position;
		glm::vec3 viewDirection;
		void updateProjectionMatrix();
		const glm::vec3 up;
};

Camera::Camera() 
	: position(2.0f, 0.0f, 0.0f),
	viewDirection(1.0f, 0.0f, 0.0f),
	up(0.0f, 1.0f, 0.0f)
{
	projectionMatrix = glm::mat4(
		vec4(1, 0, 0, 1),
		vec4(0, 1, 0, 0.0f),
		vec4(0, 0, 1, 1),
		vec4(0, 0, 0, 1)
	);
}

void Camera::updateProjectionMatrix()
{
	//projectionMatrix = glm::perspective(mFieldOfView, mAspectRatio, mNearPlaneDistance, mFarPlaneDistance);
}

void Camera::handleKeypress(int keypress) {
	if (keypress == GLFW_KEY_UP) {
		position += vec3(0.0f, 0.005f, 0.0f);
	}
	if (keypress == GLFW_KEY_DOWN) {
		position += vec3(0.0f, -0.005f, 0.0f);
	}
	if (keypress == GLFW_KEY_W) {
		position += vec3(-0.005f, 0.0f, 0.0f);
	}
	if (keypress == GLFW_KEY_S) {
		position += vec3(0.005f, 0.0f, 0.0f);
	}
	if (keypress == GLFW_KEY_A) {
		position += vec3(0.0f, 0.0f, -0.005f);
	}
	if (keypress == GLFW_KEY_D) {
		position += vec3(0.0f, 0.0f, 0.005f);
	}
}

glm::mat4 Camera::getWorldToViewMatrix() const
{
	return glm::lookAt(position, position + viewDirection, up);
}

#endif