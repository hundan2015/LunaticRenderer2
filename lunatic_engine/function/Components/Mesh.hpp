#pragma once

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
#include "../core/content/MeshContent.h"
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
    int mesh_num;
    std::shared_ptr<lunatic_engine::MeshContent> mesh_content = nullptr;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Mesh, mesh_dir, mesh_num);
static RegisterHelper<Mesh> mesh_helper("Mesh");
}  // namespace lunatic_engine
