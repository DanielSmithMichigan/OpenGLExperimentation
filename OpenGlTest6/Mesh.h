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
	void InitializeUniforms();
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
	OGLVariable* worldMatrixUniform;
	OGLVariable* viewMatrixUniform;
	OGLVariable* projectionMatrixUniform;
	glm::mat4 position = glm::translate(vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 scale = glm::scale(vec3(1.0f, 1.0f, 1.0f));
	glm::mat4 rotate = glm::rotate(0.0f, vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 worldMatrix;
	float specularPower;
	Light* ambientLight;
};

Mesh::Mesh(aiMesh &mesh) : indexBuffer(0), vertexBuffer(0), vertexArrayObject(0), samplerState(0), specularPower(40.0f) {
	worldMatrix = position * scale * rotate;
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

void Mesh::InitializeUniforms() {
	viewMatrixUniform = new OGLVariable("ViewMatrix", programHandle);
	worldMatrixUniform = new OGLVariable("WorldMatrix", programHandle);
	projectionMatrixUniform = new OGLVariable("ProjectionMatrix", programHandle);
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

	*viewMatrixUniform << objects->camera->getViewMatrix();
	*projectionMatrixUniform << objects->camera->projectionMatrix;
	*worldMatrixUniform << worldMatrix;

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
