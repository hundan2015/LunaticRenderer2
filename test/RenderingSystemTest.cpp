#include <iostream>
#include <memory>
#include "../lunatic_engine/function/Components/RenderSystem.hpp"
int main() {
    std::shared_ptr<lunatic_engine::RenderingCore> renderingManager =
        std::make_shared<lunatic_engine::RenderingCore>();
    lunatic_engine::RenderingSystem renderingSystem(renderingManager);
    std::cout << "Rendering system test.\n";
}
