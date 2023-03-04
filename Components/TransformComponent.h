#pragma once
#include <glm/fwd.hpp>
#include <nlohmann/detail/macro_scope.hpp>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include "EntityComponentSystem.hpp"
#include <glm/glm.hpp>

namespace LunaticEngine {
// Need a viewmodel of the Componnet.
/**
 * @brief Transform store an Entity's position, rotation and scale information.
 *
 */
class Transform : public Component {
   public:
    Transform() = default;
    float positonX;
    float positonY;
    float positonZ;
    float scaleX;
    float scaleY;
    float scaleZ;
    // Quartanion rotation.
    float rotationQx;
    float rotationQy;
    float rotationQz;
    float rotationQw;
    static RegisterComponent<Transform> registerComponent;
};
RegisterComponent<Transform> Transform::registerComponent =
    RegisterComponent<Transform>();

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Transform, positonX, positonY, positonZ,
                                   scaleX, scaleY, scaleZ, rotationQw,
                                   rotationQx, rotationQy, rotationQz)

class TransformManager {
   public:
    TransformManager() = delete;
    static void move(Transform &transform, const float posX, const float posY,
                     const float posZ) {
        transform.positonX = posX;
        transform.positonX = posY;
        transform.positonX = posZ;
    }
    static void move(Transform &transform, const glm::vec3 position) {
        transform.positonX = position.x;
        transform.positonX = position.y;
        transform.positonX = position.z;
    }

    static void scale(Transform &transform, const float scaleX,
                      const float scaleY, const float scaleZ) {
        transform.scaleX = scaleX;
        transform.scaleY = scaleY;
        transform.scaleZ = scaleZ;
    }
    static void scale(Transform &transform, const glm::vec3 scale) {
        transform.scaleX = scale.x;
        transform.scaleX = scale.y;
        transform.scaleX = scale.z;
    }
};
}  // namespace LunaticEngine