#include "pch.h"
#include "Mesh.h"

namespace PR
{
	Mesh Mesh::FullScreenMesh{ "FullScreenMesh",
		{
			{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0}, {0.0f, 0.0f}},
			{{-1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, -1.0}, {0.0f, 1.0f}},
			{{1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, -1.0}, {1.0f, 0.0f}},
			{{1.0f,  1.0f, 0.0f}, {0.0f, 0.0f, -1.0}, {1.0f, 1.0f}},
		},
		{0, 1, 2, 2, 1, 3} 
	};

	Mesh::Mesh(const std::string name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	{
		m_Vertices = vertices;
		m_Indices = indices;
	}

	void Mesh::Bind()
	{
		UploadMeshData();
		if (m_Upload)
			m_VertexArray->Bind();
	}

	void Mesh::Unbind() const
	{
		if (m_Upload)
			m_VertexArray->Unbind();
	}

	void Mesh::UploadMeshData()
	{
		if (!m_Upload)
		{
			m_Upload = true;

			m_VertexArray = VertexArray::Create();
			m_VertexBuffer = VertexBuffer::Create(m_Vertices.data(), static_cast<uint32_t>(m_Vertices.size()) * sizeof(Vertex));
			m_VertexBuffer->SetLayout(
				{
					{ ShaderDataType::Float3, "Position" },
					{ ShaderDataType::Float3, "Normal" },
					{ ShaderDataType::Float2, "UV0" },
					{ ShaderDataType::Float3, "Tangent" },
					{ ShaderDataType::Float3, "Bitangent" },
					{ ShaderDataType::Int4, "BoneIDs" },//TDDO,数组类型
					{ ShaderDataType::Float4, "Weights" }

				});
			m_IndexBuffer = IndexBuffer::Create(m_Indices.data(), static_cast<uint32_t>(m_Indices.size()));
			m_VertexArray->AddVertexBuffer(m_VertexBuffer);
			m_VertexArray->SetIndexBuffer(m_IndexBuffer);
		}
	}
}
