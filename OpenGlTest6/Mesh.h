#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Component.h"
#include "Error.h"
#include <Soil.h>
using namespace std;

class Mesh : public Component {
public:
	Mesh(aiMesh &mesh);
	void Initialize();
	void Draw(Camera* camera);
private:
	std::vector<VertexPositionTexture> vertices;
	std::vector<int> indices;
	std::vector<glm::vec2> textureCoordinates;
	int indexCount;
	void Mesh::createBuffers();
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;
	GLuint texture;
	GLint worldViewProjectionLocation;
	GLint projectionMatrixLocation;
	glm::mat4 mWorldMatrix;
};

Mesh::Mesh(aiMesh &mesh) : indexBuffer(0), vertexBuffer(0), vertexArrayObject(0) {
	aiVector3D *textureCoordinates = mesh.mTextureCoords[0];
	for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
		aiVector3D v = mesh.mVertices[i];
		vertices.push_back(VertexPositionTexture(vec4(v.x, v.y, v.z, 1.0f), vec2(textureCoordinates[i].x, textureCoordinates[i].y)));
	}
	if (mesh.HasFaces()) {
		for (unsigned int i = 0; i < mesh.mNumFaces; i++) {
			aiFace* face = &mesh.mFaces[i];
			for (unsigned int j = 0; j < face->mNumIndices; j++) {
				indices.push_back(face->mIndices[j]);
			}
		}
	}
}

void Mesh::Initialize() {
	createBuffers();
}

void Mesh::createBuffers() {
	enum VertexAttribute
	{
		vertexAttributePosition,
		vertexAttributeTexture
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionTexture) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Load the texture
	texture = SOIL_load_OGL_texture("Assets\\checkerboard.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB);
	if (texture == 0)
	{
		cout << SOIL_last_result();
		Error::showError("SOIL_load_OGL_texture() failed.", true);
	}

	glVertexAttribPointer(vertexAttributePosition, 4, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, Position));
	glEnableVertexAttribArray(vertexAttributePosition);

	glVertexAttribPointer(vertexAttributeTexture, 2, GL_FLOAT, GL_FALSE, sizeof(VertexPositionTexture), (void*)offsetof(VertexPositionTexture, TextureCoordinates));
	glEnableVertexAttribArray(vertexAttributeTexture);

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

void Mesh::Draw(Camera* camera) {
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindTexture(GL_TEXTURE_2D, texture);

	glUseProgram(ProgramHandle::getProgramHandle());
	mat4 wvp = camera->getWorldToViewMatrix() * mWorldMatrix;
	glUniformMatrix4fv(worldViewProjectionLocation, 1, GL_FALSE, &wvp[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &camera->projectionMatrix[0][0]);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
