#pragma once
#include "../Components/Transform.h"
#include "../System.hpp"
namespace lunatic_engine {
class TestSystem : public System {
   public:
    TestSystem();
    void OnTick(float deltaTime) override;
};
}  // namespace lunatic_engine