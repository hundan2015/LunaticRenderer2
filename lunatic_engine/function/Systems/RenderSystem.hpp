#pragma once
#include <vcruntime_typeinfo.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include "../../core/RenderingCore.h"
#include "../Component.hpp"
#include "../Components/Material.hpp"
#include "../Components/Mesh.hpp"
#include "../Components/Transform.h"
#include "../Entity.hpp"
#include "../EntityManager.hpp"
#include "../System.hpp"
#include "CameraSystem.h"
#include "glad/glad.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
using json = nlohmann::json;
// TODO(Symbolic): Remade the directory!!!!
namespace lunatic_engine {



class RenderingSystem : public System {
    std::shared_ptr<Transform>& GetTransformFromParent(
        std::shared_ptr<Transform>& transform,
        const std::shared_ptr<Transform>& transformParent) const {
        transform->position_x += transformParent->position_x;
        transform->position_y += transformParent->position_y;
        transform->position_z += transformParent->position_z;
        transform->scale_x *= transformParent->scale_x;
        transform->scale_y *= transformParent->scale_y;
        transform->scale_z *= transformParent->scale_z;
        glm::qua<float> rotate =
            glm::qua<float>(transform->rotation_w, transform->rotation_x,
                            transform->rotation_y, transform->rotation_z) *
            glm::qua<float>(
                transformParent->rotation_w, transformParent->rotation_x,
                transformParent->rotation_y, transformParent->rotation_z);
        transform->rotation_w = rotate.w;
        transform->rotation_x = rotate.x;
        transform->rotation_y = rotate.y;
        transform->rotation_z = rotate.z;
        return transform;
    }

   public:
    RenderingSystem() : System(typeid(RenderingSystem).name()) {
        required_components = {typeid(Mesh).name(), typeid(Transform).name()};
    }
    explicit RenderingSystem(
        const std::shared_ptr<RenderingCore>& renderingManagerPtr)
        : System(typeid(RenderingSystem).name()) {
        required_components = {typeid(Mesh).name(), typeid(Transform).name(),
                               typeid(Material).name()};
        mRenderingManager = renderingManagerPtr;
    }
    std::weak_ptr<EntityManager> mEntityManager;
    std::weak_ptr<RenderingCore> mRenderingManager;

    void OnStart() override {}

    void OnTick(float deltaTime) override {
        std::cout << "Rendering system is ticking." << deltaTime << std::endl;
        for (const auto& entity : targets_) {
            if (!IsEntityEnabled(entity)) continue;
            std::shared_ptr<Mesh> mesh = entity->GetComponent<Mesh>();
            std::shared_ptr<Material> material =
                entity->GetComponent<Material>();
            // Because an Entity have only a weak_ptr parent, which can't be
            // nullptr. So I transfer it into the shared_ptr first.
            std::shared_ptr<Entity> parent = entity->parent.lock();
            std::shared_ptr<Transform> transform;
            if (parent == nullptr) {
                transform = entity->GetComponent<Transform>();
            } else {
                // transform = std::make_shared<Transform>();
                auto get_world_transform =
                    [&](const std::shared_ptr<Entity>& lastEntity) {
                        std::shared_ptr<Entity> parent_entity =
                            lastEntity->parent.lock();
                        std::shared_ptr<Transform> transform_com =
                            lastEntity->GetComponent<Transform>();
                        if (parent_entity == nullptr) {
                            // You need get a new transform but not
                            // get the ptr!
                            transform =
                                std::make_shared<Transform>(*(transform_com));
                        } else {
                            transform = GetTransformFromParent(transform,
                                                               transform_com);
                        }
                        return;
                    };
                get_world_transform(parent);
            }

            // Calculate Transform matrix.
            glm::mat4 transform_mat(1.0F);
            transform_mat = glm::translate(glm::vec3(transform->position_x,
                                                     transform->position_y,
                                                     transform->position_z)) *
                            transform_mat;
            glm::mat4 rotation_mat =
                glm::mat4_cast(glm::qua<float>(
                    transform->rotation_w, transform->rotation_x,
                    transform->rotation_y, transform->rotation_z)) *
                glm::mat4(1.0);
            auto scale_mat = glm::scale(glm::vec3(
                transform->scale_x, transform->scale_y, transform->scale_z));
            auto model = transform_mat * scale_mat * rotation_mat;
            // Transfer the data to the shader.
            glm::mat4 view = CameraContext::view_;
            glm::mat4 perspective = CameraContext::perspective_;
            auto final_mat = perspective * view * model;
            // Maybe the rendering function should belong to the rendering core?
            auto rendering_function = [=]() {
                glUseProgram(material->shader_content->shader_program);
                material->SetMaterial();
                glBindVertexArray(mesh->mesh_content->vao);
                glDrawElements(GL_TRIANGLES, mesh->mesh_content->triangle_count,
                               GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);
            };
            mRenderingManager.lock()->InsertRenderCommandGroup(
                rendering_function);
        }
    }
    void OnDisabled() override {}
};  // namespace lunatic_engine
}  // namespace lunatic_engine