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

namespace lunatic_engine {

class ResourceCore {
   public:
    std::shared_ptr<RenderingCore> rendering_core;
    /**
     *
     * @param vertex_shader_dir Vertex shader's dir.
     * @param fragment_shader_dir Fragment shader's dir.
     * @return The shader content created by core.
     * @brief Get shader context from the shader dir.
     */
    [[nodiscard]] std::shared_ptr<lunatic_engine::ShaderContent>
    GetShaderContent(const std::string& vertex_shader_dir,
                     const std::string& fragment_shader_dir) const;
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
    ImageContent GetImageContent(std::string directory);
};

}  // namespace lunatic_engine
