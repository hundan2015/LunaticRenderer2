#include "CameraSystem.h"

namespace LunaticEngine {


void CameraSystem::onTick(float /*deltaTime*/) {
    for (const auto &entity : mTargets_) {
        std::shared_ptr<MainCamera> mainCamera =
            entity->getComponent<MainCamera>();
        std::shared_ptr<Transform> transform =
            entity->getComponent<Transform>();
        glm::vec3 position = glm::vec3(
            transform->mPositonX, transform->mPositonY, transform->mPositonZ);
        glm::qua<float> rotation =
            glm::qua<float>(transform->mRotationQw, transform->mRotationQx,
                            transform->mRotationQy, transform->mRotationQz);
        glm::vec3 upVector = rotation * glm::vec3(0, 1, 0);
        glm::vec3 rightVector = rotation * glm::vec3(1, 0, 0);
        glm::vec3 frontVector = rotation * glm::vec3(0, 0, 1);

        camera_context::mView_ =
            glm::lookAt(rightVector, upVector, frontVector) *
            glm::translate(position);
    }
}
CameraSystem::CameraSystem() : System(typeid(CameraSystem).name()) {
    mRequiredComponents = {typeid(MainCamera).name(), typeid(Transform).name()};
}
}  // namespace LunaticEngine