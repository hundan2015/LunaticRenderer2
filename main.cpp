#include <iostream>
#include <memory>
#include <string>
#define DEBUG_BARRIER
#include "Systems/LunaticEngineBody.h"

using LunaticEngine::RenderingManager;
// void im_gui_render_function(GLFWwindow* window);
int main(int, char**) {
    // RenderingManager::mRenderingManagerSingletonRef = nullptr;
    std::cout << "Hello, world?>Blade!\n";
    LunaticEngine::LunaticEngineBody body;
    body.startEngine();
    return 0;
}
