#pragma once
#include "glm/fwd.hpp"
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include "EntityComponentSystem.hpp"
#include <glm/glm.hpp>

namespace lunatic_engine {
// Need a viewmodel of the Componnet.
/**
 * @brief Transform store an Entity's position, rotation and scale information.
 *
 */
class Transform : public Component {
   public:
    Transform() = default;
    float mPositonX;
    float mPositonY;
    float mPositonZ;
    float mScaleX;
    float mScaleY;
    float mScaleZ;
    // Quartanion rotation.
    float mRotationQx;
    float mRotationQy;
    float mRotationQz;
    float mRotationQw;
    // static RegisterComponent<Transform> mRegisterComponent_;
};
/* RegisterComponent<Transform> Transform::mRegisterComponent_ =
    RegisterComponent<Transform>(); */

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Transform, mPositonX, mPositonY, mPositonZ,
                                   mScaleX, mScaleY, mScaleZ, mRotationQw,
                                   mRotationQx, mRotationQy, mRotationQz)

class TransformManager {
   public:
    TransformManager() = delete;
    static void move(Transform &transform, const float kPosX, const float kPosY,
                     const float kPosZ) {
        transform.mPositonX = kPosX;
        transform.mPositonX = kPosY;
        transform.mPositonX = kPosZ;
    }
    static void move(Transform &transform, const glm::vec3 kPosition) {
        transform.mPositonX = kPosition.x;
        transform.mPositonX = kPosition.y;
        transform.mPositonX = kPosition.z;
    }

    static void scale(Transform &transform, const float kScaleX,
                      const float kScaleY, const float kScaleZ) {
        transform.mScaleX = kScaleX;
        transform.mScaleY = kScaleY;
        transform.mScaleZ = kScaleZ;
    }
    static void scale(Transform &transform, const glm::vec3 kScale) {
        transform.mScaleX = kScale.x;
        transform.mScaleX = kScale.y;
        transform.mScaleX = kScale.z;
    }
};
}  // namespace lunatic_engine