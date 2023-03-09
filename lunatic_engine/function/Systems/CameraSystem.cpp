#include "CameraSystem.h"

namespace lunatic_engine {

void CameraSystem::OnTick(float /*deltaTime*/) {
    for (const auto &entity : targets_) {
        std::shared_ptr<MainCamera> main_camera =
            entity->GetComponent<MainCamera>();
        std::shared_ptr<Transform> transform =
            entity->GetComponent<Transform>();
        glm::vec3 position =
            glm::vec3(transform->position_x, transform->position_y,
                      transform->position_z);
        glm::qua<float> rotation =
            glm::qua<float>(transform->rotation_w, transform->rotation_x,
                            transform->rotation_y, transform->rotation_z);
        glm::vec3 up_vector = rotation * glm::vec3(0, 1, 0);
        glm::vec3 right_vector = rotation * glm::vec3(1, 0, 0);
        glm::vec3 front_vector = rotation * glm::vec3(0, 0, 1);

        // The View matrix of the MVP.
        // TODO: The camera context can have more.
        CameraContext::view_ =
            glm::lookAt(right_vector, up_vector, front_vector) *
            glm::translate(-position);
    }
}
CameraSystem::CameraSystem() : System(typeid(CameraSystem).name()) {
    required_components = {typeid(MainCamera).name(), typeid(Transform).name()};
}
glm::mat4 CameraContext::perspective_ = glm::mat4(1);
glm::mat4 CameraContext::view_ = glm::mat4(1);
}  // namespace lunatic_engine