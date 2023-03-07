#include <iostream>
#include <memory>
#include "../lunatic_engine/function/Systems/RenderSystem.hpp"
int main() {
    std::shared_ptr<lunatic_engine::RenderingCore> rendering_manager =
        std::make_shared<lunatic_engine::RenderingCore>();
    lunatic_engine::RenderingSystem rendering_system(rendering_manager);
    auto render_command_test = []() { std::cout << "Ticking!" << std::endl; };
    rendering_manager->InsertRenderCommandGroup(render_command_test);
    rendering_manager->InitOpenGL();
    while (rendering_manager->IsRenderEnabled()) {
        rendering_manager->RenderTick();
        rendering_manager->SwapRenderingQueue();
    }
    std::cout << "Rendering system test.\n";
}
