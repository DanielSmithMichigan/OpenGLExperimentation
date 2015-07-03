#include "Component.h"
using namespace std;

class Light : public Component {
public:
	Light(const vec4 &color, const vec3 &direction);
	Light(const vec4 &color);
	Light();
	void Initialize();
	void Update();
	glm::vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 direction = vec3(0.0f, 0.0f, 1.0f);
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

Light::Light(const vec4 &color, const vec3 &direction)
	:color(color), direction(direction)
{}

Light::Light(const vec4 &color)
	: color(color)
{}

Light::Light()
{}

void Light::Initialize() {

}

void Light::Update() {

}

