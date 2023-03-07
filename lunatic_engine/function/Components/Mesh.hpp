#pragma once

#include <vcruntime_typeinfo.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include "../../core/RenderingCore.h"
#include "../Component.hpp"
#include "../EntityManager.hpp"
#include "../System.hpp"
#include "../Systems/CameraSystem.h"
#include "Transform.h"
#include "glad/glad.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
namespace lunatic_engine {
class Mesh : public Component {
   private:
    void InitComponent() override {}

   public:
    // static RegisterComponent<Mesh> RegisterComponent;
    std::string mesh_dir;
    std::shared_ptr<lunatic_engine::MeshContent> mesh_content;
};
inline void to_json(json& jsonObject, const Mesh& mesh) {
    jsonObject = json{{"meshDir", mesh.mesh_dir}};
}
inline void from_json(const json& jsonObject, Mesh& mesh) {
    jsonObject.at("meshDir").get_to(mesh.mesh_dir);
    // OpenGL Functions
}
}  // namespace lunatic_engine
