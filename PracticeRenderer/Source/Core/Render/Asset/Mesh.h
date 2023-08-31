#pragma once

#include "Core/Render/VertexArray.h"
#include "Core/Render/VertexBuffer.h"
#include "Core/Render/IndexBuffer.h"

#include <glm/glm.hpp>

namespace PR
{

#define MAX_BONE 4

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int BoneIDs[MAX_BONE];
        //weights from each bone
        float Weights[MAX_BONE];
    };

    class Mesh 
    {
    public:
        Mesh(const std::string name, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

        void Bind();
        void Unbind() const;
        uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_Indices.size()); }
        const std::string& GetName() const { return m_Name; }

        static Mesh FullScreenMesh;
    private:
        void UploadMeshData();

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<uint32_t> m_Indices;

        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;

        std::string m_Name;

        bool m_Upload = false;
    };
}

