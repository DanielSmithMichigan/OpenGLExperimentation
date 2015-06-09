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
#include <math.h>

class Camera {
	public:
		Camera(int width, int height);
		glm::mat4 getWorldToViewMatrix() const;
		glm::mat4 projectionMatrix;
		void handleKeypress(int keypress);
		void handleMouseMovement(double offsetX, double offsetY);
	private:
		glm::vec3 position;
		glm::vec3 viewDirection;
		void updateProjectionMatrix();
		const glm::vec3 up;
		double mouseSpeed; // degrees per pixel
		double theta;
		double phi;
		double fieldOfView;
		double aspectRatio;
		double nearField;
		double farField;
};

Camera::Camera(int width, int height)
	: position(2.0f, 0.0f, 0.0f),
	viewDirection(1.0f, 0.0f, 0.0f),
	up(0.0f, 1.0f, 0.0f),
	mouseSpeed(0.005f),
	theta(0),
	phi(0),
	fieldOfView(45.0f),
	nearField(0.1f),
	farField(1000.0f)
{
	aspectRatio = double(width) / double(height);
	updateProjectionMatrix();
}

void Camera::handleMouseMovement(double offsetX, double offsetY) {
	theta += mouseSpeed * offsetX;
	phi += mouseSpeed * offsetY;
	double x = sin(phi) * cos(theta);
	double y = cos(phi);
	double z = sin(phi) * sin(theta);
	viewDirection = vec3(x, y, z);
}

void Camera::updateProjectionMatrix()
{
	projectionMatrix = glm::perspective(fieldOfView, aspectRatio, nearField, farField);
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