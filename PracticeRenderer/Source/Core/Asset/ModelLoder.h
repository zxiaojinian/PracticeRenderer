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
	class Scene;

	class ModelLoder
	{
	public:
		GameObject* LoadModel(const std::string& path, Scene* scene);
		std::vector<std::shared_ptr<Mesh>> LoadMeshes(const std::string& path);

	private:
		void ProcessNode(GameObject* parent, aiNode* node, const aiScene* aiscene, Scene* scene, std::vector<std::shared_ptr<Mesh>>& meshes, std::vector<std::shared_ptr<Material>>& materials);
		void LoadMeshes(std::vector<std::shared_ptr<Mesh>>& meshes, const aiScene* aiscene);
		void CreateMaterials(const std::string& path, std::vector<std::shared_ptr<Material>>& materials, const aiScene* aiscene);

		aiLight* IsLight(const aiScene* aiscene, const char* name);
		aiCamera* IsCamera(const aiScene* aiscene, const char* name);
	private:
		std::unordered_map<std::string, GameObject*> m_ModelCache;
	};
}

