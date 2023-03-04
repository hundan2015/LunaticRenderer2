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
using json = nlohmann::json;

namespace LunaticEngine {
class Mesh : public Component {
   private:
    void initComponent() override {}

   public:
    static RegisterComponent<Mesh> registerComponent;
    std::string mMeshDir;
    unsigned int mVao;
    unsigned int mTriangleCount;
};
// How to register a object?
// HACK: A temporary register way.
RegisterComponent<Mesh> Mesh::registerComponent = RegisterComponent<Mesh>();

void to_json(json& j, const Mesh& p) { j = json{{"meshDir", p.mMeshDir}}; }
void from_json(const json& j, Mesh& p) {
    j.at("meshDir").get_to(p.mMeshDir);
    // OpenGL Functions
}

class RenderingSystem : public System {
   public:
    RenderingSystem() : System(typeid(RenderingSystem).name()) {
        mRequiredComponents = {typeid(Mesh).name(), typeid(Transform).name()};
    }

    void onStart() override {}
    void onTick(float deltaTime) override {
        // shared_ptr<Entity> camera = context.getCamera();
        // shared_ptr<Transform> cameraTransform =
        // camera.getComponent<Tranform>();
        //
        std::cout << "Rendering system is ticking." << std::endl;
        for (auto& entity : mTargets) {
            std::shared_ptr<Mesh> mesh = entity->getComponent<Mesh>();
            // Because an Entity have only a weak_ptr parent, which can't be
            // nullptr. So I transfer it into the shared_ptr first.
            std::shared_ptr<Entity> parent = entity->mParent.lock();
            std::shared_ptr<Transform> transform;
            if (parent == nullptr) {
                transform = entity->getComponent<Transform>();
            } else {
                // transform = std::make_shared<Transform>();
                auto getWorldTransform =
                    [&](std::shared_ptr<Entity> lastEntity) {
                        std::shared_ptr<Entity> parentEntity =
                            lastEntity->mParent.lock();
                        if (parentEntity == nullptr) {
                            transform = std::make_shared<Transform>(
                                *(lastEntity->getComponent<Transform>()));
                            return;
                        } else {
                            // TODO:Do something to the transform.
                            return;
                        }
                    };
                getWorldTransform(parent);
            }

            // Caculate Transform matrix.
            glm::mat4 transfromMat(1.0f);
            // Transfer the data to the shader.
        }
    }
    void onDisabled() override {}
};  // namespace LunaticEngine
}  // namespace LunaticEngine