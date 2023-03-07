#include <iostream>
#include <memory>
#include <string>
#define DEBUG_BARRIER
#include "core/LunaticEngine.h"

using lunatic_engine::RenderingManager;
// void im_gui_render_function(GLFWwindow* window);
int main(int, char**) {
    // RenderingManager::mRenderingManagerSingletonRef = nullptr;
    std::cout << "Hello, world?>Blade!\n";
    lunatic_engine::LunaticEngine body;
    body.startEngine();
    return 0;
}
