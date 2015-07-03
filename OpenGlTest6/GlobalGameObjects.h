#include <glm/glm.hpp>
#include "Camera.h"
#include "Light.h"
#include "Colors.h"

class GlobalGameObjects {
public:
	Camera *camera;
	Light *ambientLight;
	Light *sun;
	GlobalGameObjects(int width, int height);
};

GlobalGameObjects::GlobalGameObjects(int width, int height)
	: camera(new Camera(width, height)),
	ambientLight(new Light(Colors::DimGray)),
	sun(new Light(Colors::Daylight, glm::normalize(glm::vec3(-0.8f, -1.0f, -0.6f)))) {}



