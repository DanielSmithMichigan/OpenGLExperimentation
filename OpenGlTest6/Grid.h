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
	VertexPositionTextureNormal *vertices;
	std::vector<int> indices;
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;
	GLuint samplerState;
	GLint worldViewProjectionLocation;
	GLint worldMatrixLocation;
	GLint projectionMatrixLocation;
	GLint lightDirectionLocation;
	GLuint texture;
	GLint lightColorLocation;
	GLint ambientColorLocation;
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
	vertices = new VertexPositionTextureNormal[numVertices];
	float left = (float)width / 2;
	float increment = (float)width / (float)numDivisions;
	for (int i = 0; i < numVertices; i += 4) {
		vertices[i] = VertexPositionTextureNormal(vec4(
			left, 
			0.0f,
			left - (i * increment / 4),
			1.0f), 
			Colors::RedInTexture
			,vec3(0.0f, 0.0f, 0.0f));
		vertices[i + 1] = VertexPositionTextureNormal(vec4(
			-1 * left,
			0.0f,
			left - (i * increment / 4),
			1.0f), 
			Colors::RedInTexture
			, vec3(0.0f, 0.0f, 0.0f));

		vertices[i + 2] = VertexPositionTextureNormal(vec4(
			left - (i * increment / 4),
			0.0f,
			left,
			1.0f), 
			Colors::RedInTexture
			, vec3(0.0f, 0.0f, 0.0f));
		vertices[i + 3] = VertexPositionTextureNormal(vec4(
			left - (i * increment / 4),
			0.0f,
			-1 * left,
			1.0f), 
			Colors::RedInTexture
			, vec3(0.0f, 0.0f, 0.0f));
	}

}

void Grid::createBuffers() {
	enum VertexAttribute
	{
		vertexAttributePosition,
		vertexAttributeTexture,
		vertexAttributeNormal
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionTextureNormal) * numVertices, &vertices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Load the texture
	texture = SOIL_load_OGL_texture("Assets\\checkerboard.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB);
	if (texture == 0)
	{
		cout << SOIL_last_result();
		Error::showError("SOIL_load_OGL_texture() failed.", true);
	}

	glVertexAttribPointer(vertexAttributePosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Position));
	glEnableVertexAttribArray(vertexAttributePosition);

	glVertexAttribPointer(vertexAttributeTexture, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, TextureCoordinates));
	glEnableVertexAttribArray(vertexAttributeTexture);

	glVertexAttribPointer(vertexAttributeNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTextureNormal), (void*)offsetof(VertexPositionTextureNormal, Normal));
	glEnableVertexAttribArray(vertexAttributeNormal);

	glBindVertexArray(0);

	worldViewProjectionLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "WorldViewProjection");
	if (worldViewProjectionLocation == -1)
	{
		Error::showError("Cannot find mWorldViewProjection Uniform", true);
	}

	worldMatrixLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "World");
	if (worldMatrixLocation == -1)
	{
		Error::showError("Cannot find World Uniform", true);
	}

	projectionMatrixLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "ProjectionMatrix");
	if (projectionMatrixLocation == -1)
	{
		Error::showError("Cannot find projectMatrix Uniform", true);
	}

	ambientColorLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "AmbientColor");
	if (ambientColorLocation == -1)
	{
		Error::showError("Cannot find ambientColor Uniform", true);
	}

	lightColorLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "LightColor");
	if (lightColorLocation == -1)
	{
		Error::showError("Cannot find lightColor Uniform", true);
	}

	lightDirectionLocation = glGetUniformLocation(ProgramHandle::getProgramHandle(), "LightDirection");
	if (lightDirectionLocation == -1)
	{
		Error::showError("Cannot find lightDirection Uniform", true);
	}

	glGenSamplers(1, &samplerState);
	glSamplerParameteri(samplerState, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glSamplerParameteri(samplerState, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glSamplerParameteri(samplerState, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameteri(samplerState, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameterf(samplerState, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);
}

void Grid::Update() {

}

void Grid::Draw(GlobalGameObjects* objects) {
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindSampler(0, samplerState);


	glUseProgram(ProgramHandle::getProgramHandle());
	mat4 wvp = objects->camera->getWorldToViewMatrix() * mWorldMatrix;
	glUniformMatrix4fv(worldViewProjectionLocation, 1, GL_FALSE, &wvp[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &objects->camera->projectionMatrix[0][0]);
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
	glUniform4fv(ambientColorLocation, 1, &objects->ambientLight->color[0]);
	glUniform4fv(lightColorLocation, 1, &objects->directionalLight->color[0]);
	glUniform3fv(lightDirectionLocation, 1, &objects->directionalLight->direction[0]);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDrawArrays(GL_LINES, 0, numVertices);
	glBindVertexArray(0);
}
