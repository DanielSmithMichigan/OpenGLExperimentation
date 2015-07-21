#include <glm/glm.hpp>
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Colors.h"

class GlobalGameObjects {
public:
	Camera *camera;
	Light *ambientLight;
	Light *sun;
	std::vector<Light*> lights;
	GlobalGameObjects(int width, int height);
private:
	void init();
};

GlobalGameObjects::GlobalGameObjects(int width, int height)
	: camera(new Camera(width, height))
{
	init();
}

void GlobalGameObjects::init() {
	sun = new Light();
	sun->color = Colors::Daylight;
	sun->direction = glm::vec3(-0.8f, -1.0f, -0.6f);
	ambientLight = new Light();
	ambientLight->color = Colors::DimGray;
	Light *lightOne = new Light();
	lightOne->color = Colors::NeonRed;
	lightOne->position = glm::vec3(4.0f, 4.0f, 4.0f);
	lightOne->radius = 5.0f;
	lights.push_back(lightOne);
	Light *lightTwo = new Light();
	lightTwo->color = Colors::Purple;
	lightTwo->position = glm::vec3(-4.0f, 4.0f, 4.0f);
	lightTwo->radius = 5.0f;
	lights.push_back(lightTwo);
}



