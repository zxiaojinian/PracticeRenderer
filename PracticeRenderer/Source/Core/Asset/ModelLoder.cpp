#include "pch.h"
#include "ModelLoder.h"

#include "Core/Log/Log.h"
#include "Core/Scene/Components/MeshRenderer.h"
#include "Core/Scene/Scene.h"
#include "Core/Asset/Resources.h"

#include <vector>

namespace PR
{
    GameObject* ModelLoder::LoadModel(const std::string& path, Scene* scene)
    {
        if (m_ModelCache.find(path) != m_ModelCache.end())
            return m_ModelCache[path];

        Assimp::Importer import;
        const aiScene* aiscene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!aiscene || aiscene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !aiscene->mRootNode)
        {
            PR_LOG_ERROR("ERROR::ASSIMP::import.GetErrorString()");
            return nullptr;
        }

        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Material>> materials;
        LoadMeshes(meshes, aiscene);
        CreateMaterials(path, materials, aiscene);

        auto lastSlash = path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = path.rfind('.');
        auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
        std::string name = path.substr(lastSlash, count);

        GameObject* root = new GameObject(name, scene);
        m_ModelCache[path] = root;
        ProcessNode(root, aiscene->mRootNode, aiscene, scene, meshes, materials);
        return root;
    }

    void ModelLoder::ProcessNode(GameObject* parent, aiNode* node, const aiScene* aiscene, Scene* scene, std::vector<std::shared_ptr<Mesh>>& meshes, std::vector<std::shared_ptr<Material>>& materials)
    {
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            auto mesh = meshes[node->mMeshes[i]];        
            auto mat = materials[aiscene->mMeshes[node->mMeshes[i]]->mMaterialIndex];
            GameObject* go = new GameObject(mesh->GetName(), scene);
            go->SetParent(parent);

            MeshRenderer& meshRender = go->AddComponent<MeshRenderer>();
            meshRender.SetMesh(mesh);
            meshRender.AddMaterial(mat);
        }

        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(parent, node->mChildren[i], aiscene, scene, meshes, materials);
        }
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

    std::string TextureTypeToShaderName(aiTextureType type)
    {
        switch (type)
        {
        case aiTextureType_DIFFUSE:
            return "u_Diffuse";
            break;
        case aiTextureType_SPECULAR:
            return "m_Specular";
            break;
        case aiTextureType_NORMALS:
            return "u_Normal";
            break;
        case aiTextureType_SHININESS:
            return "u_Roughness";
            break;
        default:
            PR_ASSERT(false, "Unknown aiTextureType");
            return "";
            break;
        }
    }

    void ModelLoder::CreateMaterials(const std::string& path, std::vector<std::shared_ptr<Material>>& materials, const aiScene* aiscene)
    {
        if (aiscene)
        {
            std::vector<aiTextureType> textureTypes = {
                aiTextureType_DIFFUSE,
                aiTextureType_NORMALS,
                aiTextureType_SPECULAR,
                aiTextureType_SHININESS,
                //aiTextureType_AMBIENT_OCCLUSION
            };

            auto directory = path.substr(0, path.find_last_of('/') + 1);
            for (unsigned int i = 0; i < aiscene->mNumMaterials; i++) 
            {
                aiMaterial* material = aiscene->mMaterials[i];
                std::shared_ptr<Material> mat = std::make_shared<Material>();
                mat->SetShader(Resources::Get().GetShader("PBRLit"));

                for (const aiTextureType& textureType : textureTypes) 
                {
                    if (material->GetTextureCount(textureType) > 0) 
                    {
                        aiString texturePath;
                        material->GetTexture(textureType, 0, &texturePath);
                        std::string fullPath = directory + texturePath.C_Str();
                        auto tex = Resources::Get().LoadTexture(fullPath);
                        auto propertyName = TextureTypeToShaderName(textureType);
                        if(propertyName != "")
                            mat->SetTexture(propertyName, tex.get());
                    }
                }
                materials.push_back(mat);
            }
        }
    }
}