#pragma once

#include <vcruntime_typeinfo.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include "../../core/RenderingCore.h"
#include "../Core/Component.hpp"
#include "../Core/EntityManager.hpp"
#include "../Core/System.hpp"
#include "../Systems/CameraSystem.h"
#include "../../core/content/MeshContent.h"
#include "Mesh.hpp"
#include "../../core/content/ShaderContent.h"
#include "../../core/content/ImageContent.h"
#include "Transform.h"
#include "glad/glad.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
namespace lunatic_engine {
class Material : public Component {
   public:
    std::string shader_vs_dir;
    std::string shader_fs_dir;
    std::shared_ptr<lunatic_engine::ShaderContent> shader_content;
    std::map<std::string, std::string> name_dir_map;
    std::map<std::string, lunatic_engine::ImageContent> name_image_content_map;
    void SetMaterial() {
        for (auto &pair : name_image_content_map) {

        }
    }
};
}  // namespace lunatic_engine

// How to register a object?
// HACK: A temporary register way.
// RegisterComponent<Mesh> Mesh::RegisterComponent = RegisterComponent<Mesh>();
