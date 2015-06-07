#include "Component.h"
using namespace std;

class CubeComponent : public Component {
public:
	CubeComponent();
	void Initialize();
	void Draw(Camera* camera);
private:
	GLuint mVertexBuffer;
	GLuint mVertexArrayObject;
	GLuint mIndexBuffer;
	GLint mWorldViewProjectionLocation;
	GLint projectionMatrixLocation;
	glm::mat4 mWorldMatrix;
};

CubeComponent::CubeComponent() {
}

void CubeComponent::Initialize() {
	enum VertexAttribute
	{
		VertexAttributePosition,
		VertexAttributeColor
	};
	VertexPositionColor vertices[] = {
		VertexPositionColor(vec4(1.0f, 1.0f, 1.0f, 1.0f), Colors::Red),
		VertexPositionColor(vec4(1.0f, 1.0f, 0.0f, 1.0f), Colors::Red),

		VertexPositionColor(vec4(1.0f, 0.0f, 1.0f, 1.0f), Colors::Blue),
		VertexPositionColor(vec4(1.0f, 0.0f, 0.0f, 1.0f), Colors::Blue),

		VertexPositionColor(vec4(0.0f, 1.0f, 1.0f, 1.0f), Colors::Green),
		VertexPositionColor(vec4(0.0f, 1.0f, 0.0f, 1.0f), Colors::Green),

		VertexPositionColor(vec4(0.0f, 0.0f, 1.0f, 1.0f), Colors::Red),
		VertexPositionColor(vec4(0.0f, 0.0f, 0.0f, 1.0f), Colors::Red)
	};

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	GLuint indices[] = {
		1, 4, 0,
		1, 5, 4,

		1, 3, 7,
		1, 7, 5,

		1, 0, 2,
		1, 2, 3,

		0, 4, 6,
		0, 6, 2,

		7, 3, 2,
		2, 6, 7,

		4, 5, 7,
		7, 6, 4
	};

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &mVertexArrayObject);
	glBindVertexArray(mVertexArrayObject);

	glVertexAttribPointer(VertexAttributePosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Position));
	glEnableVertexAttribArray(VertexAttributePosition);

	glVertexAttribPointer(VertexAttributeColor, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Color));
	glEnableVertexAttribArray(VertexAttributeColor);

	glBindVertexArray(0);

	mWorldViewProjectionLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "WorldViewProjection");
	if (mWorldViewProjectionLocation == -1) 
	{
		Error::showError("Cannot find mWorldViewProjection Uniform", true);
	}

	projectionMatrixLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "ProjectionMatrix");
	if (projectionMatrixLocation == -1)
	{
		Error::showError("Cannot find projectMatrix Uniform", true);
	}
}

void CubeComponent::Draw(Camera* camera) {
	glBindVertexArray(mVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glUseProgram(ProgramHandle::getProgramHandle());
	mat4 wvp = camera->getWorldToViewMatrix() * mWorldMatrix;
	glUniformMatrix4fv(mWorldViewProjectionLocation, 1, GL_FALSE, &wvp[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &camera->projectionMatrix[0][0]);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
