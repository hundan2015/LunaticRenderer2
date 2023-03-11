#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>
namespace lunatic_engine {
namespace model_loader {
class Mesh {
   public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> ebo_s;
    std::vector<glm::vec2> uvs;
    unsigned int triangle_count;
};
class AssimpLoader {
    std::vector<Mesh> meshes_;

   public:
    void LoadModel(const std::string &path) {
        Assimp::Importer import;
        const aiScene *scene =
            import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString()
                      << std::endl;
            return;
        }
        std::string directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
    }
    void ProcessNode(aiNode *node, const aiScene *scene) {
        // 处理节点所有的网格（如果有的话）
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes_.push_back(ProcessMesh(mesh));
        }
        // 接下来对它的子节点重复这一过程
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }
    static Mesh ProcessMesh(aiMesh *mesh) {
        Mesh res;
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            glm::vec3 vertex;
            glm::vec3 normal;
            vertex.x = mesh->mVertices[i].x;
            vertex.y = mesh->mVertices[i].y;
            vertex.z = mesh->mVertices[i].z;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            res.normals.emplace_back(normal);
            res.vertices.emplace_back(vertex);
            if (mesh->mTextureCoords[0]) {
                glm::vec2 uv;
                uv.x = mesh->mTextureCoords[0][i].x;
                uv.y = mesh->mTextureCoords[0][i].y;
                res.uvs.emplace_back(uv);
            }
        }
        res.triangle_count=mesh->mNumFaces;
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace &face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                res.ebo_s.emplace_back(face.mIndices[j]);
            }
        }
        return res;
    }
    std::vector<Mesh> &GetMeshes() { return meshes_; }
    std::vector<Mesh> &GetMeshes(const std::string &dir) {
        LoadModel(dir);
        return meshes_;
    }
};

}  // namespace model_loader

}  // namespace lunatic_engine
