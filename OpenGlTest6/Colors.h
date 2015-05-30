#include <glm/glm.hpp>

class Colors
{
public:
	static const glm::vec4 Red;
	static const glm::vec4 Green;
	static const glm::vec4 Blue;
};

const glm::vec4 Colors::Red = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 Colors::Green = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 Colors::Blue = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);