#include "Camera.h"
#include "Light.h"

class GlobalGameObjects {
public:
	Camera *camera;
	Light *ambientLight;
	Light *directionalLight;
	GlobalGameObjects(int width, int height);
};

GlobalGameObjects::GlobalGameObjects(int width, int height)
	: camera(new Camera(width, height)), ambientLight(new Light()), directionalLight(new Light()) {}



