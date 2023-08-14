#pragma once

#include "Core/Scene/Component.h"
#include "Core/Render/Asset/Mesh.h"
#include "Core/Render/Asset/Material.h"

namespace PR
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer(GameObject& gameobject);
		virtual std::string GetName() override { return "MeshRenderer"; }

		void SetMesh(const std::shared_ptr<Mesh>& mesh) { m_Mesh = mesh; }
		const std::shared_ptr<Mesh>& GetMesh() const { return m_Mesh; }

		void AddMaterial(std::shared_ptr<Material> material) { m_Materials.push_back(material); }
		void SetMaterials(std::vector<std::shared_ptr<Material>>& materials) { m_Materials = materials; }
		const std::vector<std::shared_ptr<Material>>& GetMaterials() const { return m_Materials; }
	private:
		std::shared_ptr<Mesh> m_Mesh;
		std::vector<std::shared_ptr<Material>> m_Materials;

	};
}

