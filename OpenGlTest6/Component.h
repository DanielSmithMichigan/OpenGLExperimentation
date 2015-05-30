#ifndef COMPONENT_HEADER
#define COMPONENT_HEADER
#include <GL/glew.h>
#include <glm/glm.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;
#include <glm/gtc/matrix_transform.hpp>
#include "VertexObjects.h"
#include "Colors.h"
#include "ProgramHandle.h"

using namespace std;

class Component{
	public:
		virtual void Draw() = 0;
		virtual void Initialize() = 0;
		Component();
	private:
};

Component::Component() {
}

#endif