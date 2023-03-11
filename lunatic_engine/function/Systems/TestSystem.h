#pragma once
#include "../Components/Transform.h"
#include "../Core/System.hpp"
namespace lunatic_engine {
/**
 * @brief TestSystem is a little test system to test the system manager.
 * @author Symbolic
 */
class TestSystem : public System {
   public:
    TestSystem();
    void OnTick(float deltaTime) override;
};
}  // namespace lunatic_engine