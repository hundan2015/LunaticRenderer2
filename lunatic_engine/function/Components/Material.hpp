#pragma once

#include <glm/fwd.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include "../../core/RenderingCore.h"
#include "../../core/content/ImageContent.h"
#include "../../core/content/MeshContent.h"
#include "../../core/content/ShaderContent.h"
#include "../Core/Component.hpp"
#include "../Core/EntityManager.hpp"
#include "../Core/System.hpp"
#include "../Systems/CameraSystem.h"
#include "Mesh.hpp"
#include "Transform.h"
#include "glad/glad.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"

// #define GET_ID(x) GL_TEXTURE#x
namespace lunatic_engine {
class Material : public Component {
   public:
    std::string shader_vs_dir;
    std::string shader_fs_dir;
    std::shared_ptr<lunatic_engine::ShaderContent> shader_content;
    std::map<std::string, std::string> name_dir_map;
    std::map<std::string, std::shared_ptr<lunatic_engine::ImageContent>>
        name_image_content_map;
    /**
     * @brief Going to set content to a shader.
     * @warning This function has been wrapped into a rendering command. So it
     * doesn't need to being pushed again.
     * @note In the shader file, it always like this:"uniform sampler2D
     * texture_name;" So you need to need the code below.
     */
    // TODO: Complete the material class.
    // TODO: A Material should have special nlohman init method, which using the
    // InitMaterial().
    void InitMaterial() {
        // Because of the texture id, the map after init should not change!
        int texture_id = 0;
        for (auto &pair : name_image_content_map) {
            auto texture_position = glGetUniformLocation(
                shader_content->shader_program, pair.first.c_str());
            glUniform1i(texture_position, texture_id);
            texture_id++;
        }
    }
    void SetMaterial() {
        int texture_id = 0;
        for (auto &pair : name_image_content_map) {
            // A usage provided by learn opengl.
            glActiveTexture(GL_TEXTURE0 + texture_id);
            glBindTexture(GL_TEXTURE_2D, pair.second->image);
            auto texture_position = glGetUniformLocation(
                shader_content->shader_program, pair.first.c_str());
            glUniform1i(texture_position, texture_id);
            texture_id++;
        }
    }
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Material, shader_vs_dir, shader_fs_dir,
                                   name_dir_map);
static RegisterHelper<Material> material_helper("Material");
}  // namespace lunatic_engine

// How to register a object?
// HACK: A temporary register way.
// RegisterComponent<Mesh> Mesh::RegisterComponent = RegisterComponent<Mesh>();
