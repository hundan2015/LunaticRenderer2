#include <iostream>
#include <memory>
#include "../RenderSystem.hpp"
int main() {
    std::shared_ptr<LunaticEngine::RenderingManager> renderingManager =
        std::make_shared<LunaticEngine::RenderingManager>();
    LunaticEngine::RenderingSystem renderingSystem(renderingManager);
    std::cout << "Rendering system test.\n";
}
