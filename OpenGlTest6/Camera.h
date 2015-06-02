#include <glm/glm.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
	public:
		Camera();
		glm::mat4 viewMatrix;
	private:
		void updateViewMatrix();
		glm::vec3 position;
		glm::quat rotation;
};

void Camera::updateViewMatrix() {
}
