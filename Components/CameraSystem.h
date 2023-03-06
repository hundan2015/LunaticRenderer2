#pragma once
#include <glm/detail/type_quat.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include "EntityComponentSystem.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtc/quaternion.hpp>
#include "TransformComponent.h"
namespace lunatic_engine {
class MainCamera : public Component {};
struct camera_context {
    static glm::mat4 mView_;
    static glm::mat4 mPerspective_;
};
glm::mat4 camera_context::mPerspective_ = glm::mat4(1);
glm::mat4 camera_context::mView_ = glm::mat4(1);

class CameraSystem : public System {
   public:
    CameraSystem();
    void onTick(float /*deltaTime*/) override;
};
}  // namespace lunatic_engine