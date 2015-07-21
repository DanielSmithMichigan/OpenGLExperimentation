#ifndef COMPONENT_HEADER
#define COMPONENT_HEADER
#include <GL/glew.h>
#include <glm/glm.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec2;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>
#include "VertexObjects.h"
#include "Colors.h"
#include "Error.h"

using namespace std;

class Component
{
public:
	virtual void doInitialize() = 0;
	virtual void Update() = 0;
	Component();
private:
protected:
};

Component::Component() {
}


#endif