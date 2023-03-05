#include "TestSystem.h"
void LunaticEngine::TestSystem::onTick(float deltaTime) {
    std::cout << "TestSystem is ticking." << deltaTime << std::endl;
}
LunaticEngine::TestSystem::TestSystem() : System(typeid(TestSystem).name()) {
    mRequiredComponents = {typeid(Transform).name()};
}
