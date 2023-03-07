#include <iostream>
#include <memory>
#include "../RenderSystem.hpp"
int main() {
    std::shared_ptr<lunatic_engine::RenderingManager> renderingManager =
        std::make_shared<lunatic_engine::RenderingManager>();
    lunatic_engine::RenderingSystem renderingSystem(renderingManager);
    std::cout << "Rendering system test.\n";
}
