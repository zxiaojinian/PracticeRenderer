#pragma once


#include "Core/Scene/GameObject.h"
#include "Core/Render/Asset/Mesh.h"
#include "Core/Render/Asset/Material.h"
#include "Core/Render/Texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace PR
{
	//class GameObject;
	//class Mesh;
	class Scene;

	class ModelLoder
	{
	public:
		ModelLoder();
		static ModelLoder& Get() { return *s_Instance; }

		GameObject* LoadModel(const std::string& path, Scene* scene);

	private:
		void processNode(GameObject* parent, aiNode* node, const aiScene* aiscene, Scene* scene);
		std::shared_ptr<Mesh> processMesh(aiMesh* mesh);
		void LoadMeshes(std::vector<std::shared_ptr<Mesh>>& meshes, const aiScene* aiscene);
		void CreateMaterials(std::vector<std::shared_ptr<Material>>& materials, const aiScene* aiscene);
		void LoadTextures(std::vector<std::shared_ptr<Texture>>& Textures, const aiScene* aiscene);

	private:
		static ModelLoder* s_Instance;
		std::string m_Directory;
	};
}

