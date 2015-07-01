#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "DrawableGameComponent.h"
#include <Soil.h>
using namespace std;

class Mesh : public DrawableGameComponent {
public:
	Mesh(aiMesh &mesh);
	void Initialize();
	void Draw(GlobalGameObjects* objects);
	void Update();
private:
	std::vector<VertexPositionTextureNormal> vertices;
	std::vector<int> indices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoordinates;
	int indexCount;
	void Mesh::createBuffers();
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;
	GLuint texture;
	GLuint samplerState;
	GLint worldMatrixLocation;
	GLint worldViewProjectionLocation;
	GLint projectionMatrixLocation;
	GLint ambientColorLocation;
	GLint lightColorLocation;
	GLint lightDirectionLocation;
	glm::mat4 mWorldMatrix;
	Light* ambientLight;
};

Mesh::Mesh(aiMesh &mesh) : indexBuffer(0), vertexBuffer(0), vertexArrayObject(0), samplerState(0) {
	aiVector3D *textureCoordinates = mesh.mTextureCoords[0];
	for (unsigned int i = 0; i < mesh.mNumVertices; i++) {
		aiVector3D v = mesh.mVertices[i];
		aiVector3D n = mesh.mNormals[i];
		vertices.push_back(VertexPositionTextureNormal(vec4(v.x, v.y, v.z, 1.0f), vec2(textureCoordinates[i].x, textureCoordinates[i].y), vec3(n.x, n.y, n.z)));
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
		vertexAttributeTexture,
		vertexAttributeNormal
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionTextureNormal) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

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

void Mesh::Update() {

}

void Mesh::Draw(GlobalGameObjects* objects) {
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
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

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
