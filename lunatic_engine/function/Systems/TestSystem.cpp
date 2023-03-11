#include "TestSystem.h"
void lunatic_engine::TestSystem::OnTick(float deltaTime) {
    std::cout << "TestSystem is ticking." << 1 / deltaTime << std::endl;
}
lunatic_engine::TestSystem::TestSystem() : System(typeid(TestSystem).name()) {
    required_components = {typeid(Transform).name()};
}
