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
	OGLVariable* worldMatrixUniform;
	OGLVariable* viewMatrixUniform;
	OGLVariable* projectionMatrixUniform;
	OGLVariable* ambientColorUniform;
	OGLVariable* lightColorUniform;
	OGLVariable* lightDirectionUniform;
	glm::mat4 position = glm::translate(vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 scale = glm::scale(vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 rotate = glm::rotate(0.0f, vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 worldMatrix;
	GLuint texture;
};

Grid::Grid(int numDivisions, int width) : indexBuffer(0), vertexBuffer(0), vertexArrayObject(0), numDivisions(numDivisions), width(width) {
	worldMatrix = position * scale * rotate;

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

	viewMatrixUniform = new OGLVariable("ViewMatrix");
	worldMatrixUniform = new OGLVariable("WorldMatrix");
	projectionMatrixUniform = new OGLVariable("ProjectionMatrix");
	ambientColorUniform = new OGLVariable("AmbientColor");
	lightColorUniform = new OGLVariable("LightColor");
	lightDirectionUniform = new OGLVariable("LightDirection");

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

	*viewMatrixUniform << objects->camera->getViewMatrix();
	*projectionMatrixUniform << objects->camera->projectionMatrix;
	*worldMatrixUniform << worldMatrix;
	*ambientColorUniform << objects->ambientLight->color;
	*lightColorUniform << objects->sun->color;
	*lightDirectionUniform << objects->sun->direction;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDrawArrays(GL_LINES, 0, numVertices);
	glBindVertexArray(0);
}
