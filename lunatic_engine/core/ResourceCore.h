#pragma once
#include <iostream>
#include <sstream>
#include "AssimpLoader.h"
#include "RenderingCore.h"
#include "content/ImageContent.h"
#include "content/MeshContent.h"
#include "content/ShaderContent.h"
#include "fstream"
#include "glad/glad.h"
#include "map"

namespace lunatic_engine {

class ResourceCore {
    std::mutex mesh_info_map_mutex_;
    std::map<std::string, std::shared_ptr<MeshInfo>> mesh_info_map_;
    std::mutex image_content_map_mutex_;
    std::map<std::string, std::shared_ptr<ImageContent>> image_content_map_;
    std::mutex shader_content_map_mutex_;
    std::map<std::string, std::shared_ptr<ShaderContent>> shader_content_map_;
    lunatic_engine::model_loader::AssimpLoader assimp_loader;

   public:
    std::shared_ptr<RenderingCore> rendering_core;
    /**
     * @param vertex_shader_dir Vertex shader's dir.
     * @param fragment_shader_dir Fragment shader's dir.
     * @return The shader content created by core.
     * @brief Get shader context from the shader dir.
     */
    [[nodiscard]] std::shared_ptr<lunatic_engine::ShaderContent>
    GetShaderContent(const std::string& vertex_shader_dir,
                     const std::string& fragment_shader_dir);
    static std::string GetShaderFileString(const std::string& shader_dir);
    /**
     *
     * @param shader The shader's handle. Including vertex shader and fragment
     * shader.
     * @param type A enum including {'PROGRAM','VERTEX','FRAGMENT'}
     * @brief Checking the errors of shader, and print them.
     */
    static void CheckCompileErrors(GLuint shader, const std::string& type);

    MeshContent GetMeshContent(model_loader::Mesh mesh);
    ImageContent GetImageContent(const std::string& image_dir);

    std::shared_ptr<MeshInfo> GetMeshInfo(std::string model_dir) {
        {
            std::lock_guard mesh_info_lock_guard(mesh_info_map_mutex_);
            auto find_result = mesh_info_map_.find(model_dir);
            if (find_result != mesh_info_map_.end()) {
                return find_result->second;
            }
        }
        auto mesh_node_root = assimp_loader.GetMeshNode(model_dir);
        int counter = 0;
        auto result = std::make_shared<MeshInfo>();
        auto root = DFSMeshInfo(mesh_node_root, counter, result->mesh_list);
        result->root = root;
        {
            std::lock_guard mesh_info_lock_guard(mesh_info_map_mutex_);
            mesh_info_map_.insert(std::make_pair(model_dir, result));
        }
        return result;
    }
    std::shared_ptr<MeshContentNode> DFSMeshInfo(
        std::shared_ptr<model_loader::MeshNode>& mesh_node, int& counter,
        std::vector<std::shared_ptr<MeshContent>>& mesh_list) {
        auto res = std::make_shared<MeshContentNode>();
        res->num = counter;
        counter++;
        if (mesh_node->mesh) {
            res->mesh_content = std::make_shared<MeshContent>(
                GetMeshContent(*(mesh_node->mesh)));
            mesh_list.emplace_back(res->mesh_content);
        }

        for (auto child : mesh_node->child) {
            res->children.emplace_back(DFSMeshInfo(child, counter, mesh_list));
        }
        return res;
    }
};

}  // namespace lunatic_engine
