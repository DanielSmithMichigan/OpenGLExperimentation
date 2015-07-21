#include "Component.h"
using namespace std;

class Light : public Component {
public:
	Light();
	void doInitialize();
	void Update();
	glm::vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 direction = vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 position = vec3(0.0f, 0.0f, 0.0f);
	float radius = 5.0f;
private:
};

Light::Light()
{

}

void Light::doInitialize()
{

}

void Light::Update()
{

}

