#include "pch.h"
#include "ModelLoder.h"

#include "Core/Log/Log.h"
#include "Core/Scene/Components/MeshRenderer.h"
#include "Core/Asset/ShaderLibrary.h"

#include "Core/Scene/SceneManager.h"
#include "Core/Scene/GameObject.h"
#include "Core/Render/Asset/Mesh.h"

#include <vector>

namespace PR
{
    ModelLoder* ModelLoder::s_Instance = nullptr;

    ModelLoder::ModelLoder()
    {
        s_Instance = this;
    }

    GameObject* ModelLoder::LoadModel(const std::string& path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            PR_LOG_ERROR("ERROR::ASSIMP::import.GetErrorString()");
            return nullptr;
        }
        m_Directory = path.substr(0, path.find_last_of('/'));

        GameObject* root = new GameObject(m_Directory);
        processNode(root, scene->mRootNode, scene);
        return root;
    }

    void ModelLoder::processNode(GameObject* parent, aiNode* node, const aiScene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            auto mesh = processMesh(scene->mMeshes[node->mMeshes[i]], scene);
            GameObject* go = new GameObject(mesh->GetName());
            go->SetParent(parent);

            MeshRenderer& meshRender = go->AddComponent<MeshRenderer>();
            meshRender.SetMesh(std::shared_ptr<Mesh>(mesh));
            std::shared_ptr<Shader> shader = Shader::Create("Assets/FlatColor.glsl");
            std::shared_ptr<Material> mat = std::make_shared<Material>("Test mat");
            mat->SetShader(shader);
            meshRender.AddMaterial(mat);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(parent, node->mChildren[i], scene);
        }
    }

    Mesh* ModelLoder::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            //normal
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }

            if (mesh->mTextureCoords[0])
            {
                //uv
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            if (mesh->HasTangentsAndBitangents())
            {
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;

                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            vertices.push_back(vertex);
        }

        //indices
        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return new Mesh(mesh->mName.C_Str(), vertices, indices);
    }
}