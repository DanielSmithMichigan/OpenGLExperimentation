#include <glm/glm.hpp>
using glm::mat3;
using glm::mat4;
using glm::vec3;
using glm::vec4;

class VertexPositionColor
{
public:
	glm::vec4 Position;
	glm::vec4 Color;
	VertexPositionColor() {};
	VertexPositionColor(const glm::vec4& position, const glm::vec4& color)
		: Position(position), Color(color) { }
};

class VertexPositionTextureNormal
{
public:
	glm::vec4 Position;
	glm::vec2 TextureCoordinates;
	glm::vec3 Normal;
	VertexPositionTextureNormal() {};
	VertexPositionTextureNormal(const glm::vec4& position, const glm::vec2& textureCoordinates, const glm::vec3& normal)
		: Position(position), TextureCoordinates(textureCoordinates) , Normal(normal){}
};
