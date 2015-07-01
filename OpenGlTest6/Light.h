#include "Component.h"
using namespace std;

class Light : public Component {
public:
	Light();
	void Initialize();
	void Update();
	glm::vec4 color;
	glm::vec3 direction;
private:
	GLuint mVertexBuffer;
	GLuint mVertexArrayObject;
	GLuint mIndexBuffer;
	GLint mWorldViewProjectionLocation;
	GLint projectionMatrixLocation;
	glm::vec3 up;
	glm::vec3 right;
	glm::mat4 mWorldMatrix;
};

Light::Light()
{
	color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	direction = vec3(-1.0f, 0.0f, 0.0f);
	up = vec3(0.0f, 0.0f, 0.0f);
	right = vec3(0.0f, 0.0f, 0.0f);
}

void Light::Initialize() {

}

void Light::Update() {

}

