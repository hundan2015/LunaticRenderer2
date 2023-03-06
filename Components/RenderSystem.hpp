#pragma once
#include <vcruntime_typeinfo.h>
#include "EntityComponentSystem.hpp"
#include <glm/fwd.hpp>
#include <iostream>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include "TransformComponent.h"
#include "EntityManager.hpp"
#include "CameraSystem.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include "../Systems/RenderingManager.h"
#include "glad/glad.h"
using json = nlohmann::json;
// TODO(Symbolic): Remade the directory!!!!
namespace lunatic_engine {
class RenderingManager;
class Mesh : public Component {
   private:
    void initComponent() override {}

   public:
    // static RegisterComponent<Mesh> registerComponent;
    std::string mMeshDir;
    unsigned int mVao;
    unsigned int mTriangleCount;
};
class Material : public Component {
   public:
    unsigned int mShaderProgram;
    virtual void setMaterial();
};
// How to register a object?
// HACK: A temporary register way.
// RegisterComponent<Mesh> Mesh::registerComponent = RegisterComponent<Mesh>();

inline void to_json(json& jsonObject, const Mesh& mesh) {
    jsonObject = json{{"meshDir", mesh.mMeshDir}};
}
inline void from_json(const json& jsonObject, Mesh& mesh) {
    jsonObject.at("meshDir").get_to(mesh.mMeshDir);
    // OpenGL Functions
}

class RenderingSystem : public System {
   public:
    RenderingSystem() : System(typeid(RenderingSystem).name()) {
        mRequiredComponents = {typeid(Mesh).name(), typeid(Transform).name()};
    }
    explicit RenderingSystem(
        const std::shared_ptr<RenderingManager>& renderingManagerPtr)
        : System(typeid(RenderingSystem).name()) {
        mRequiredComponents = {typeid(Mesh).name(), typeid(Transform).name(),
                               typeid(Material).name()};
        mRenderingManager = renderingManagerPtr;
    }
    std::weak_ptr<EntityManager> mEntityManager;
    std::weak_ptr<RenderingManager> mRenderingManager;

    void onStart() override {}
    void onTick(float deltaTime) override {
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
            std::shared_ptr<Entity> parent = entity->mParent.lock();
            std::shared_ptr<Transform> transform;
            if (parent == nullptr) {
                transform = entity->getComponent<Transform>();
            } else {
                // transform = std::make_shared<Transform>();
                auto getWorldTransform =
                    [&](const std::shared_ptr<Entity>& lastEntity) {
                        std::shared_ptr<Entity> parentEntity =
                            lastEntity->mParent.lock();
                        std::shared_ptr<Transform> transformCom =
                            lastEntity->getComponent<Transform>();
                        if (parentEntity == nullptr) {
                            transform =
                                std::make_shared<Transform>(*(transformCom));
                        } else {
                            transform->mPositonX += transformCom->mPositonX;
                            transform->mPositonY += transformCom->mPositonY;
                            transform->mPositonZ += transformCom->mPositonZ;
                            transform->mScaleX *= transformCom->mScaleX;
                            transform->mScaleY *= transformCom->mScaleY;
                            transform->mScaleZ *= transformCom->mScaleZ;
                            glm::qua<float> rotate =
                                glm::qua<float>(transform->mRotationQw,
                                                transform->mRotationQx,
                                                transform->mRotationQy,
                                                transform->mRotationQz) *
                                glm::qua<float>(transformCom->mRotationQw,
                                                transformCom->mRotationQx,
                                                transformCom->mRotationQy,
                                                transformCom->mRotationQz);
                            transform->mRotationQw = rotate.w;
                            transform->mRotationQx = rotate.x;
                            transform->mRotationQy = rotate.y;
                            transform->mRotationQz = rotate.z;
                        }
                        return;
                    };
                getWorldTransform(parent);
            }

            // Caculate Transform matrix.
            glm::mat4 transformMat(1.0F);
            transformMat = glm::translate(glm::vec3(transform->mPositonX,
                                                    transform->mPositonY,
                                                    transform->mPositonZ)) *
                           transformMat;
            glm::mat4 rotationMat =
                glm::mat4_cast(glm::qua<float>(
                    transform->mRotationQw, transform->mRotationQx,
                    transform->mRotationQy, transform->mRotationQz)) *
                glm::mat4(1.0);
            auto scaleMat = glm::scale(glm::vec3(
                transform->mScaleX, transform->mScaleY, transform->mScaleZ));
            auto model = transformMat * scaleMat * rotationMat;
            // Transfer the data to the shader.
            glm::mat4 view = camera_context::mView_;
            glm::mat4 perspective = camera_context::mPerspective_;
            auto finalMat = perspective * view * model;

            auto renderingFunction = [=]() {
                glUseProgram(material->mShaderProgram);
                material->setMaterial();
                glBindVertexArray(mesh->mVao);
                glDrawElements(GL_TRIANGLES, mesh->mTriangleCount,
                               GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);
            };
            mRenderingManager.lock()->insertRenderCommandGroup(
                renderingFunction);
        }
    }
    void onDisabled() override {}
};  // namespace lunatic_engine
}  // namespace lunatic_engine