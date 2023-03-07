#pragma once
#include "EntityComponentSystem.hpp"
#include "TransformComponent.h"
namespace lunatic_engine {
class TestSystem : public System {
   public:
    TestSystem();
    void OnTick(float deltaTime) override;
};
}  // namespace lunatic_engine