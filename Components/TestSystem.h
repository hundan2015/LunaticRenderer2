#pragma once
#include "EntityComponentSystem.hpp"
#include "TransformComponent.h"
namespace LunaticEngine {
class TestSystem : public System {
   public:
    TestSystem() : System(typeid(TestSystem).name()) {
        mRequiredComponents = {typeid(Transform).name()};
    }
    void onTick(float deltaTime) override {
        std::cout << "TestSystem is ticking." << std::endl;
    }
};
}  // namespace LunaticEngine