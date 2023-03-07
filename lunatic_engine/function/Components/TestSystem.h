#pragma once
#include "EntityComponentSystem.hpp"
#include "TransformComponent.h"
namespace lunatic_engine {
class TestSystem : public System {
   public:
    TestSystem();
    void onTick(float deltaTime) override;
};
}  // namespace lunatic_engine