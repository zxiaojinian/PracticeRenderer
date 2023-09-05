#include "pch.h"
#include "ModelLoder.h"

#include "Core/Log/Log.h"
#include "Core/Scene/Components/MeshRenderer.h"
#include "Core/Asset/ShaderLibrary.h"
#include "Core/Scene/Scene.h"

#include <vector>

namespace PR
{
    ModelLoder* ModelLoder::s_Instance = nullptr;

    ModelLoder::ModelLoder()
    {
        s_Instance = this;
    }

    GameObject* ModelLoder::LoadModel(const std::string& path, Scene* scene)
    {
        Assimp::Importer import;
        const aiScene* aiscene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode)
        {
            PR_LOG_ERROR("ERROR::ASSIMP::import.GetErrorString()");
            return nullptr;
        }
        m_Directory = path.substr(0, path.find_last_of('/'));

        auto lastSlash = path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = path.rfind('.');
        auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        std::string name = path.substr(lastSlash, count);

        GameObject* root = new GameObject(name, scene);
        processNode(root, aiscene->mRootNode, aiscene, scene);
        return root;
    }

    void ModelLoder::processNode(GameObject* parent, aiNode* node, const aiScene* aiscene, Scene* scene)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            auto mesh = processMesh(aiscene->mMeshes[node->mMeshes[i]]);
            GameObject* go = new GameObject(mesh->GetName(), scene);
            go->SetParent(parent);

            MeshRenderer& meshRender = go->AddComponent<MeshRenderer>();
            meshRender.SetMesh(std::shared_ptr<Mesh>(mesh));
            std::shared_ptr<Shader> shader = Shader::Create("Assets/SimpleLit.glsl");
            std::shared_ptr<Material> mat = std::make_shared<Material>("Test mat");
            mat->SetShader(shader);
            meshRender.AddMaterial(mat);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(parent, node->mChildren[i], aiscene, scene);
        }
    }

    std::shared_ptr<Mesh> ModelLoder::processMesh(aiMesh* mesh)
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

        return std::make_shared<Mesh>(mesh->mName.C_Str(), vertices, indices);
    }

    void ModelLoder::LoadMeshes(std::vector<std::shared_ptr<Mesh>>& meshes, const aiScene* aiscene)
    {
        if (aiscene)
        {
            for (unsigned int i = 0; i < aiscene->mNumMeshes; i++)
            {
                auto mesh = aiscene->mMeshes[i];
                if (mesh)
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
                    meshes.push_back(std::make_shared<Mesh>(mesh->mName.C_Str(), vertices, indices));
                }
            }
        }
    }

    void ModelLoder::CreateMaterials(std::vector<std::shared_ptr<Material>>& materials, const aiScene* aiscene)
    {

    }

    void ModelLoder::LoadTextures(std::vector<std::shared_ptr<Texture>>& Textures, const aiScene* aiscene)
    {

    }
}