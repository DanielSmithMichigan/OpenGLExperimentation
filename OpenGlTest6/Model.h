#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Error.h"
#include "Mesh.h"
#include <vector>

class Model {
	public:
		Model(char* fileName);
		void pushMeshesTo(std::vector<Component*> &components);
	private:
		std::vector<Mesh*> meshes;
};

Model::Model(char* fileName) {
	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
	const aiScene *scene = importer.ReadFile(fileName, flags);
	if (scene == nullptr) {
		Error::showError("Error creating program object.\n", true);
	}
	if (scene->HasMeshes()) {
		for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
			Mesh* mesh = new Mesh(*(scene->mMeshes[i]));
			meshes.push_back(mesh);
		}
	} 
}

void Model::pushMeshesTo(std::vector<Component*> &components) {
	for (Mesh* mesh : meshes)
	{
		components.push_back(mesh);
	}
}