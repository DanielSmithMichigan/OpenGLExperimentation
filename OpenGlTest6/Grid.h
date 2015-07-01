#include <vector>
#include "DrawableGameComponent.h"
using namespace std;

class Grid : public DrawableGameComponent {
public:
	Grid(int numRows, int widthRow);
	void Initialize();
	void Draw(GlobalGameObjects* objects);
	void Update();
private:
	void createBuffers();
	void createGrid();
	int numDivisions;
	int numVertices;
	int width;
	VertexPositionColor *vertices;
	std::vector<int> indices;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;
	GLint worldViewProjectionLocation;
	GLint projectionMatrixLocation;
	glm::mat4 mWorldMatrix;
};

Grid::Grid(int numDivisions, int width) : indexBuffer(0), vertexBuffer(0), vertexArrayObject(0), numDivisions(numDivisions), width(width) {

}

void Grid::Initialize() {
	createGrid();
	createBuffers();
}

void Grid::createGrid() {
	numVertices = (numDivisions + 1) * 4;
	vertices = new VertexPositionColor[numVertices];
	float left = (float)width / 2;
	float increment = (float)width / (float)numDivisions;
	for (int i = 0; i < numVertices; i += 4) {
		vertices[i] = VertexPositionColor(vec4(
			left, 
			0.0f,
			left - (i * increment / 4),
			1.0f), Colors::Red);
		vertices[i + 1] = VertexPositionColor(vec4(
			-1 * left,
			0.0f,
			left - (i * increment / 4),
			1.0f), Colors::Red);

		vertices[i + 2] = VertexPositionColor(vec4(
			left - (i * increment / 4),
			0.0f,
			left,
			1.0f), Colors::Red);
		vertices[i + 3] = VertexPositionColor(vec4(
			left - (i * increment / 4),
			0.0f,
			-1 * left,
			1.0f), Colors::Red);
	}
}

void Grid::createBuffers() {
	enum VertexAttribute
	{
		vertexAttributePosition,
		vertexAttributeColor
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionColor) * numVertices, &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	glVertexAttribPointer(vertexAttributePosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Position));
	glEnableVertexAttribArray(vertexAttributePosition);

	glVertexAttribPointer(vertexAttributeColor, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionColor), (void*)offsetof(VertexPositionColor, Color));
	glEnableVertexAttribArray(vertexAttributeColor);

	glBindVertexArray(0);

	worldViewProjectionLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "WorldViewProjection");
	if (worldViewProjectionLocation == -1)
	{
		Error::showError("Cannot find mWorldViewProjection Uniform", true);
	}

	projectionMatrixLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "ProjectionMatrix");
	if (projectionMatrixLocation == -1)
	{
		Error::showError("Cannot find projectMatrix Uniform", true);
	}
}

void Grid::Update() {

}

void Grid::Draw(GlobalGameObjects* objects) {
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glUseProgram(ProgramHandle::getProgramHandle());
	mat4 wvp = objects->camera->getWorldToViewMatrix() * mWorldMatrix;
	glUniformMatrix4fv(worldViewProjectionLocation, 1, GL_FALSE, &wvp[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &objects->camera->projectionMatrix[0][0]);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDrawArrays(GL_LINES, 0, numVertices);
	glBindVertexArray(0);
}
