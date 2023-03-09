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
/**
 * CameraContext should be a new type like a Notification.
 * This type of "component" is shared in different systems.
 * These component doesn't belong to any entity. These context should be POD.
 */
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