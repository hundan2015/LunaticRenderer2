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

    std::shared_ptr<MeshContent> GetMeshContent(model_loader::Mesh mesh);
    std::shared_ptr<ImageContent> GetImageContent(const std::string& image_dir);

    std::shared_ptr<MeshInfo> GetMeshInfo(std::string model_dir);
    std::shared_ptr<MeshContentNode> DFSMeshInfo(
        std::shared_ptr<model_loader::MeshNode>& mesh_node, int& counter,
        std::vector<std::shared_ptr<MeshContent>>& mesh_list,
        const std::string& mesh_dir);
};

}  // namespace lunatic_engine
