#include "TestSystem.h"
void lunatic_engine::TestSystem::onTick(float deltaTime) {
    std::cout << "TestSystem is ticking." << deltaTime << std::endl;
}
lunatic_engine::TestSystem::TestSystem() : System(typeid(TestSystem).name()) {
    mRequiredComponents = {typeid(Transform).name()};
}
