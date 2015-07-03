#ifndef COLORS_HANDLE
#define COLORS_HANDLE
#include <glm/glm.hpp>
#include <stdlib.h>

class Colors
{
public:
	static const glm::vec2 RedInTexture;
	static const glm::vec4 Daylight;
	static const glm::vec4 DimGray;
	static const glm::vec4 Black;
	static const glm::vec4 White;
	static const glm::vec4 Random();
};

const glm::vec2 Colors::RedInTexture = glm::vec2(0.0f, 0.0f);
const glm::vec4 Colors::Daylight = glm::vec4(1.0f, float(228) / float(255), float(206) / float(255), 1.0f);
const glm::vec4 Colors::DimGray = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
const glm::vec4 Colors::Black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 Colors::White = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 Colors::Random() {
	return glm::vec4(
		float(rand() % 100) / 100.0f,
		float(rand() % 100) / 100.0f,
		float(rand() % 100) / 100.0f,
		1.0f
	);

}

#endif