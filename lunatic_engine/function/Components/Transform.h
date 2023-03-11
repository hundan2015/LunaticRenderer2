#pragma once
#include <glm/glm.hpp>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include "../Core/Component.hpp"
#include "../Core/System.hpp"
#include "glm/fwd.hpp"

namespace lunatic_engine {
// Need a viewmodel of the Componnet.
/**
 * @brief Transform store an Entity's position, rotation and scale information.
 *
 */
class Transform : public Component {
   public:
    Transform() = default;
    float position_x;
    float position_y;
    float position_z;
    float scale_x;
    float scale_y;
    float scale_z;
    // Quartanion rotation.
    float rotation_x;
    float rotation_y;
    float rotation_z;
    float rotation_w;
    // static RegisterComponent<Transform> mRegisterComponent_;
};
/* RegisterComponent<Transform> Transform::mRegisterComponent_ =
    RegisterComponent<Transform>(); */

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Transform, position_x, position_y,
                                   position_z,
                                   scale_x, scale_y, scale_z, rotation_w,
                                   rotation_x, rotation_y, rotation_z)

class TransformManager {
   public:
    TransformManager() = delete;
    static void move(Transform &transform, const float kPosX, const float kPosY,
                     const float kPosZ) {
        transform.position_x = kPosX;
        transform.position_x = kPosY;
        transform.position_x = kPosZ;
    }
    static void move(Transform &transform, const glm::vec3 kPosition) {
        transform.position_x = kPosition.x;
        transform.position_x = kPosition.y;
        transform.position_x = kPosition.z;
    }

    static void scale(Transform &transform, const float kScaleX,
                      const float kScaleY, const float kScaleZ) {
        transform.scale_x = kScaleX;
        transform.scale_y = kScaleY;
        transform.scale_z = kScaleZ;
    }
    static void scale(Transform &transform, const glm::vec3 kScale) {
        transform.scale_x = kScale.x;
        transform.scale_y = kScale.y;
        transform.scale_z = kScale.z;
    }
};
}  // namespace lunatic_engine