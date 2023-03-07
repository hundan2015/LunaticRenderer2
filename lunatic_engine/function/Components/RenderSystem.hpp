#pragma once
#include <vcruntime_typeinfo.h>
#include <glm/fwd.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include "../../core/RenderingCore.h"
#include "CameraSystem.h"
#include "EntityComponentSystem.hpp"
#include "EntityManager.hpp"
#include "TransformComponent.h"
#include "glad/glad.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
using json = nlohmann::json;
// TODO(Symbolic): Remade the directory!!!!
namespace lunatic_engine {
class RenderingManager;
class Mesh : public Component {
   private:
    void InitComponent() override {}

   public:
    // static RegisterComponent<Mesh> RegisterComponent;
    std::string mesh_dir;
    unsigned int vao;
    unsigned int triangle_count;
};
class Material : public Component {
   public:
    unsigned int shader_program;
    virtual void SetMaterial();
};
// How to register a object?
// HACK: A temporary register way.
// RegisterComponent<Mesh> Mesh::RegisterComponent = RegisterComponent<Mesh>();

inline void to_json(json& jsonObject, const Mesh& mesh) {
    jsonObject = json{{"meshDir", mesh.mesh_dir}};
}
inline void from_json(const json& jsonObject, Mesh& mesh) {
    jsonObject.at("meshDir").get_to(mesh.mesh_dir);
    // OpenGL Functions
}

class RenderingSystem : public System {
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
        // shared_ptr<Entity> camera = context.getCamera();
        // shared_ptr<Transform> cameraTransform =
        // camera.getComponent<Tranform>();
        //
        std::cout << "Rendering system is ticking." << deltaTime << std::endl;
        for (const auto& entity : mTargets_) {
            std::shared_ptr<Mesh> mesh = entity->getComponent<Mesh>();
            std::shared_ptr<Material> material =
                entity->getComponent<Material>();
            // Because an Entity have only a weak_ptr parent, which can't be
            // nullptr. So I transfer it into the shared_ptr first.
            std::shared_ptr<Entity> parent = entity->parent.lock();
            std::shared_ptr<Transform> transform;
            if (parent == nullptr) {
                transform = entity->getComponent<Transform>();
            } else {
                // transform = std::make_shared<Transform>();
                auto get_world_transform =
                    [&](const std::shared_ptr<Entity>& lastEntity) {
                        std::shared_ptr<Entity> parentEntity =
                            lastEntity->parent.lock();
                        std::shared_ptr<Transform> transformCom =
                            lastEntity->getComponent<Transform>();
                        if (parentEntity == nullptr) {
                            transform =
                                std::make_shared<Transform>(*(transformCom));
                        } else {
                            transform->position_x += transformCom->position_x;
                            transform->position_y += transformCom->position_y;
                            transform->position_z += transformCom->position_z;
                            transform->scale_x *= transformCom->scale_x;
                            transform->scale_y *= transformCom->scale_y;
                            transform->scale_z *= transformCom->scale_z;
                            glm::qua<float> rotate =
                                glm::qua<float>(transform->rotation_w,
                                                transform->rotation_x,
                                                transform->rotation_y,
                                                transform->rotation_z) *
                                glm::qua<float>(transformCom->rotation_w,
                                                transformCom->rotation_x,
                                                transformCom->rotation_y,
                                                transformCom->rotation_z);
                            transform->rotation_w = rotate.w;
                            transform->rotation_x = rotate.x;
                            transform->rotation_y = rotate.y;
                            transform->rotation_z = rotate.z;
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
            glm::mat4 view = camera_context::view_;
            glm::mat4 perspective = camera_context::mPerspective_;
            auto final_mat = perspective * view * model;

            auto rendering_function = [=]() {
                glUseProgram(material->shader_program);
                material->SetMaterial();
                glBindVertexArray(mesh->vao);
                glDrawElements(GL_TRIANGLES, mesh->triangle_count,
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