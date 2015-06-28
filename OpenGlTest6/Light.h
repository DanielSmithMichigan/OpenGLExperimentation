#include "Component.h"
using namespace std;

class Light : public Component {
public:
	Light();
	void Initialize();
	void Draw(Camera* camera);
	glm::vec4 color;
private:
	GLuint mVertexBuffer;
	GLuint mVertexArrayObject;
	GLuint mIndexBuffer;
	GLint mWorldViewProjectionLocation;
	GLint projectionMatrixLocation;
	glm::mat4 mWorldMatrix;
};

Light::Light() {
	color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}

void Light::Initialize() {

}

void Light::Draw(Camera* camera) {
}
