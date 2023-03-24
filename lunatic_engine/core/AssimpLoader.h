#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <memory>
#include <vector>
namespace lunatic_engine {
namespace model_loader {
class Mesh {
   public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> ebo_s;
    std::vector<glm::vec2> uvs;
    unsigned int triangle_count = 0;
};

class MeshNode {
   public:
    std::shared_ptr<Mesh> mesh = nullptr;
    std::vector<std::shared_ptr<MeshNode>> child;
};
// TODO: Here need some design! The mesh info is too foolish!
class MeshInfo {
   public:
    std::shared_ptr<MeshNode> root;
    std::vector<std::shared_ptr<Mesh>> mesh_list;
};
class AssimpLoader {
    std::shared_ptr<MeshNode> mesh_root_;

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

        // ProcessNode(scene->mRootNode, scene);
        mesh_root_ = ProcessNodeDeco(scene->mRootNode, scene);
    }
    std::shared_ptr<MeshNode> ProcessNodeDeco(
        aiNode *node, const aiScene *scene,
        std::shared_ptr<MeshInfo> mesh_info = nullptr) {
        auto mesh_node = std::make_shared<MeshNode>();
        auto mesh_temp = Mesh();
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            mesh_temp = ProcessMesh(mesh, mesh_temp);
        }
        if (!mesh_temp.vertices.empty()) {
            mesh_node->mesh = std::make_shared<Mesh>(mesh_temp);
            mesh_info->mesh_list.emplace_back(mesh_node->mesh);
        }
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            mesh_node->child.emplace_back(
                ProcessNodeDeco(node->mChildren[i], scene));
        }
        return mesh_node;
    }

    /**
     * @param mesh The input ai mesh.
     * @return A Mesh from Assimp loader which supported by the OpenGL.
     */
    static Mesh ProcessMesh(aiMesh *mesh, Mesh res = Mesh()) {
        // Mesh res;
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
        res.triangle_count += mesh->mNumFaces;
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace &face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; ++j) {
                res.ebo_s.emplace_back(face.mIndices[j]);
            }
        }
        return res;
    }

    std::shared_ptr<MeshNode> GetMeshNode(const std::string &dir) {
        LoadModel(dir);
        return mesh_root_;
    }
    std::shared_ptr<MeshInfo> GetMeshInfo(const std::string &dir) {
        auto mesh_node_root = GetMeshNode(dir);
        auto res = std::make_shared<MeshInfo>();
        res->root = mesh_node_root;
        DFSFunction(mesh_node_root, 0, res);
        return res;
    }
    void DFSFunction(std::shared_ptr<MeshNode> root, int counter,
                     std::shared_ptr<MeshInfo> &res) {
        res->mesh_list.emplace_back(root->mesh);
        for (auto &child_mesh : root->child) {
            DFSFunction(child_mesh, counter + 1, res);
        }
    }
};

}  // namespace model_loader

}  // namespace lunatic_engine
