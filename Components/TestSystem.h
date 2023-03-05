#pragma once
#include "EntityComponentSystem.hpp"
#include "TransformComponent.h"
namespace LunaticEngine {
class TestSystem : public System {
   public:
    TestSystem();
    void onTick(float deltaTime) override;
};
}  // namespace LunaticEngine