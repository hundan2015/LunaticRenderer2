#pragma once
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include "../../core/RenderingCore.h"
#include "../../core/ResourceCore.h"
#include "../Components/Material.hpp"
#include "../Components/Mesh.hpp"
#include "../Components/Transform.h"
#include "../Core/Component.hpp"
#include "../Core/Entity.hpp"
#include "../Core/EntityManager.hpp"
#include "../Core/System.hpp"
#include "CameraSystem.h"
#include "glad/glad.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
using json = nlohmann::json;
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
        const std::shared_ptr<RenderingCore>& rendering_core_ptr)
        : System(typeid(RenderingSystem).name()) {
        required_components = {typeid(Mesh).name(), typeid(Transform).name(),
                               typeid(Material).name()};
        rendering_core = rendering_core_ptr;
    }
    std::weak_ptr<EntityManager> entity_manager;
    std::weak_ptr<RenderingCore> rendering_core;
    std::weak_ptr<ResourceCore> resource_core;

    void OnStart() override {}
    void GetWorldTransform(std::shared_ptr<Transform>& transform,
                           std::shared_ptr<Entity> last_entity) {
        std::shared_ptr<Entity> parent_entity = last_entity->parent.lock();
        std::shared_ptr<Transform> transform_com =
            last_entity->GetComponent<Transform>();
        if (parent_entity == nullptr) {
            // You need get a new transform but not
            // get the ptr!
            transform = std::make_shared<Transform>(*(transform_com));
        } else {
            GetWorldTransform(transform, parent_entity);
            transform = GetTransformFromParent(transform, transform_com);
        }
    }
    void OnTick(float deltaTime) override {
        std::cout << "Rendering system is ticking." << deltaTime << std::endl;
        for (const auto& entity : targets_) {
            if (!IsEntityEnabled(entity)) continue;
            std::shared_ptr<Mesh> mesh = entity->GetComponent<Mesh>();
            std::shared_ptr<Material> material =
                entity->GetComponent<Material>();
            // Init Mesh and material component.
            if (mesh->mesh_content == nullptr) {
                // TODO: make a mesh content getter.
                auto resource_share_ptr = resource_core.lock();
                if (!resource_share_ptr) {
                    std::cout << "ERROR::Resource core fucked up!";
                    abort();
                }
                auto mesh_info =
                    resource_share_ptr->GetMeshInfo(mesh->mesh_dir);
                mesh->mesh_content = mesh_info->mesh_list[mesh->mesh_num];
            }
            if (material->shader_content == nullptr) {
                material->shader_content =
                    resource_core.lock()->GetShaderContent(
                        material->shader_vs_dir, material->shader_fs_dir);
            }
            if (material->name_image_content_map.empty()) {
                // TODO:TEST here!
                auto resource_share_ptr = resource_core.lock();
                if (!resource_share_ptr) {
                    std::cout << "ERROR::Resource core fucked up!";
                    abort();
                }
                for (auto& name : material->name_dir_map) {
                    auto image_content =
                        resource_share_ptr->GetImageContent(name.second);
                    material->name_image_content_map.insert(
                        std::make_pair(name.first, image_content));
                }
            }
            // Because an Entity have only a weak_ptr parent, which can't be
            // nullptr. So I transfer it into the shared_ptr first.
            std::shared_ptr<Entity> parent = entity->parent.lock();
            std::shared_ptr<Transform> transform;
            // This part is trying to apply an entity's parents and
            // grandparent's transform.

            if (parent == nullptr) {
                transform = entity->GetComponent<Transform>();
            } else {
                // transform = std::make_shared<Transform>();
                GetWorldTransform(transform, entity);
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
            // glm::mat4 perspective = CameraContext::perspective_;
            glm::mat4 perspective = glm::perspective(
                glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
            auto final_mat = perspective * view * model;

            // Maybe the rendering function should belong to the rendering core?
            auto rendering_function = [=]() {
                glUseProgram(material->shader_content->shader_program);
                int mat_mvp_location = glGetUniformLocation(
                    material->shader_content->shader_program, "mat_mvp");
                glUniformMatrix4fv(mat_mvp_location, 1, GL_FALSE,
                                   glm::value_ptr(final_mat));
                material->SetMaterial();
                glBindVertexArray(mesh->mesh_content->vao);
                std::cout << "Mesh VAO:" << mesh->mesh_content->vao
                          << std::endl;
                glDrawElements(GL_TRIANGLES,
                               mesh->mesh_content->triangle_count * 3,
                               GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);
            };

            rendering_core.lock()->InsertRenderCommandGroup(rendering_function);
        }
    }
    void OnDisabled() override {}
};  // namespace lunatic_engine
}  // namespace lunatic_engine