#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Component.h"
#include "Error.h"
using namespace std;

class Mesh : public Component {
public:
	Mesh(aiMesh *mesh);
	void Initialize();
	void Draw(Camera* camera);
private:
	aiMesh *mesh;
	std::vector<glm::vec3> vertices;
	std::vector<int> indices;
	int indexCount;
	void Mesh::createBuffers();
	GLuint vertexBuffer;
	GLuint indexBuffer;
	GLuint vertexArrayObject;
	GLint worldViewProjectionLocation;
	GLint projectionMatrixLocation;
	glm::mat4 mWorldMatrix;
};

Mesh::Mesh(aiMesh *mesh) : mesh(mesh), indexBuffer(0), vertexBuffer(0), vertexArrayObject(0) {
}

void Mesh::Initialize() {
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		aiVector3D v = mesh->mVertices[i];
		vertices.push_back(glm::vec3(v.x, v.y, v.z));
	}
	if (mesh->HasFaces()) {
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			aiFace* face = &mesh->mFaces[i];
			for (unsigned int j = 0; j < face->mNumIndices; j++) {
				indices.push_back(face->mIndices[j]);
			}
		}
	}
	createBuffers();
}

void Mesh::createBuffers() {
	enum VertexAttribute
	{
		vertexAttributePosition,
		vertexAttributeColor
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexPositionColor) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * indices.size(), &indices[0], GL_STATIC_DRAW);

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

void Mesh::Draw(Camera* camera) {
	glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glUseProgram(ProgramHandle::getProgramHandle());
	mat4 wvp = camera->getWorldToViewMatrix() * mWorldMatrix;
	glUniformMatrix4fv(worldViewProjectionLocation, 1, GL_FALSE, &wvp[0][0]);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &camera->projectionMatrix[0][0]);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
