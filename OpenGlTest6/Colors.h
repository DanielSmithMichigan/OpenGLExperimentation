#ifndef COLORS_HANDLE
#define COLORS_HANDLE
#include <glm/glm.hpp>
#include <stdlib.h>

class Colors
{
public:
	static const glm::vec4 Black;
	static const glm::vec4 Daylight;
	static const glm::vec4 DimGray;
	static const glm::vec4 NeonRed;
	static const glm::vec4 Purple;
	static const glm::vec4 Random();
	static const glm::vec2 RedInTexture;
	static const glm::vec4 White;
private:
	static const glm::vec4 toVec4(int red, int green, int blue);
};

const glm::vec4 Colors::Black = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 Colors::Daylight = Colors::toVec4(255, 228, 206);
const glm::vec4 Colors::DimGray = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
const glm::vec4 Colors::NeonRed = Colors::toVec4(224, 21, 55);
const glm::vec4 Colors::Purple = Colors::toVec4(155, 22, 187);
const glm::vec4 Colors::Random() { return toVec4(rand() % 255, rand() % 255, rand() % 255); }
const glm::vec2 Colors::RedInTexture = glm::vec2(0.0f, 0.0f);
const glm::vec4 Colors::White = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

const glm::vec4 Colors::toVec4(int red, int green, int blue) {
	return glm::vec4(
		float(red) / 255.0f,
		float(green) / 255.0f,
		float(blue) / 255.0f,
		1.0f
	);
}

#endif