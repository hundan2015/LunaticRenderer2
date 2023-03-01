#pragma once
#include <vcruntime_typeinfo.h>
#include "EntityComponentSystem.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
using json = nlohmann::json;

namespace LunaticEngine {
class Mesh : Component {
   private:
    void initComponent() override {}

   public:
    std::string mMeshDir;
    unsigned int mVao;
    unsigned int mTriangleCount;
};

void to_json(json& j, const Mesh& p) { j = json{{"meshDir", p.mMeshDir}}; }
void from_json(const json& j, Mesh& p) {
    j.at("meshDir").get_to(p.mMeshDir);
    // OpenGL Functions
}

class RenderingSystem : private System {
    std::vector<std::string> mRequiredComponents = {"Mesh"};

   public:
    void onStart() override {}
    void onTick(float deltaTime) override {
        // shared_ptr<Entity> camera = context.getCamera();
        // shared_ptr<Transform> cameraTransform =
        // camera.getComponent<Tranform>();
        //
        for (auto& entity : mTargets) {
            std::shared_ptr<Mesh> mesh = entity.second.getComponent<Mesh>();
            // std::shared_ptr<Transform> transform =
            // entity.second.getComponent<Transform>();
        }
    }
    void onDisabled() override {}
};
}  // namespace LunaticEngine