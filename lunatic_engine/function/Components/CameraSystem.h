#pragma once
#include <glm/detail/type_quat.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include "EntityComponentSystem.hpp"
#include "TransformComponent.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/glm.hpp"
namespace lunatic_engine {
class MainCamera : public Component {};
struct CameraContext {
    static glm::mat4 view_;
    static glm::mat4 perspective_;
};
glm::mat4 CameraContext::perspective_ = glm::mat4(1);
glm::mat4 CameraContext::view_ = glm::mat4(1);

class CameraSystem : public System {
   public:
    CameraSystem();
    void OnTick(float /*deltaTime*/) override;
};
}  // namespace lunatic_engine