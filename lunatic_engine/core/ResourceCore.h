﻿#pragma once
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
#include "set"

namespace lunatic_engine {

class ResourceCore {
    std::mutex mesh_info_map_mutex_;
    std::map<std::string, std::shared_ptr<MeshInfo>> mesh_info_map_;
    std::mutex image_content_map_mutex_;
    std::map<std::string, std::shared_ptr<ImageContent>> image_content_map_;
    std::mutex shader_content_map_mutex_;
    std::map<std::string, std::shared_ptr<ShaderContent>> shader_content_map_;
    lunatic_engine::model_loader::AssimpLoader assimp_loader;

    std::set<std::string> mesh_info_wait_set_;
    std::mutex mesh_info_wait_mutex_;
    std::set<std::string> image_content_wait_set_;
    std::mutex image_content_wait_mutex_;
    std::set<std::string> shader_content_wait_set_;
    std::mutex shader_content_wait_mutex_;

   public:
    std::shared_ptr<RenderingCore> rendering_core;
    /**
     * @param shader_content_map_mutex Vertex shader's dir.
     * @param fragment_shader_dir Fragment shader's dir.
     * @return The shader content created by core.
     * @brief Get shader context from the shader dir.
     */
    std::shared_ptr<lunatic_engine::ShaderContent> GetShaderContent(
        const std::string shader_content_map_mutex,
        const std::string fragment_shader_dir, bool is_immediatly);
    static std::string GetShaderFileString(const std::string& shader_dir);
    /**
     *
     * @param shader The shader's handle. Including vertex shader and fragment
     * shader.
     * @param type A enum including {'PROGRAM','VERTEX','FRAGMENT'}
     * @brief Checking the errors of shader, and print them.
     */
    static void CheckCompileErrors(GLuint shader, const std::string& type);

    std::shared_ptr<MeshContent> GetMeshContent(
        std::shared_ptr<model_loader::Mesh> mesh, bool is_blocking = false);
    std::shared_ptr<ImageContent> GetImageContent(
        const std::string& image_content_map_mutex, bool is_immediatly = true);

    std::shared_ptr<MeshInfo> GetMeshInfo(const std::string model_dir,
                                          bool is_immediatly);
    std::shared_ptr<MeshContentNode> DFSMeshInfo(
        std::shared_ptr<model_loader::MeshNode>& mesh_node, int& counter,
        std::vector<std::shared_ptr<MeshContent>>& mesh_list,
        const std::string& mesh_dir);
};

}  // namespace lunatic_engine
