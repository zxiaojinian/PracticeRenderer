#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace PR
{
	class GameObject;
	class Mesh;

	class ModelLoder
	{
	public:
		ModelLoder();
		static ModelLoder& Get() { return *s_Instance; }

		GameObject* LoadModel(const std::string& path);

	private:
		void processNode(GameObject* parent, aiNode* node, const aiScene* scene);
		Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	private:
		static ModelLoder* s_Instance;
		std::string m_Directory;
	};
}

