#pragma once
#include <glm/detail/type_quat.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <memory>
#include "../Components/Transform.h"
#include "../Core/Component.hpp"
#include "../Core/System.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/glm.hpp"
namespace lunatic_engine {
class MainCamera : public Component {};
// TODO:CameraContext should be a component!
struct CameraContext {
    static glm::mat4 view_;
    static glm::mat4 perspective_;
};


class CameraSystem : public System {
   public:
    CameraSystem();
    void OnTick(float /*deltaTime*/) override;
};
}  // namespace lunatic_engine