#include "Component.h"
using namespace std;

class TriangleComponent : public Component {
public:
	TriangleComponent();
	void Initialize();
	void Draw();
private:
	GLuint mVertexBuffer;
	GLuint mVertexArrayObject;
};

TriangleComponent :: TriangleComponent() {
}

void TriangleComponent::Initialize() {
	enum VertexAttribute
	{
		VertexAttributePosition,
		VertexAttributeColor
	};
	VertexPositionColor vertices[] = {
		VertexPositionColor(vec4(-0.8f, -0.8f, 0.0f, 1.0f), Colors::Red),
		VertexPositionColor(vec4(0.8f, -0.8f, 0.0f, 1.0f), Colors::Green),
		VertexPositionColor(vec4(0.0f, 0.8f, 0.0f, 1.0f), Colors::Blue)
	};

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &mVertexArrayObject);
	glBindVertexArray(mVertexArrayObject);

	glVertexAttribPointer(VertexAttributePosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Position));
	glEnableVertexAttribArray(VertexAttributePosition);

	glVertexAttribPointer(VertexAttributeColor, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Color));
	glEnableVertexAttribArray(VertexAttributeColor);

	glBindVertexArray(0);
}

void TriangleComponent::Draw() {
	GLuint programHandle = ProgramHandle::getProgramHandle();
	glBindVertexArray(mVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glUseProgram(programHandle);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
