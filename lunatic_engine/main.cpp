#include <iostream>
#include "core/LunaticEngine.h"

using lunatic_engine::RenderingCore;
// void im_gui_render_function(GLFWwindow* window);
int main(int, char**) {
    // RenderingCore::mRenderingManagerSingletonRef = nullptr;
    std::cout << "Hello, world?>Blade!\n";
    lunatic_engine::LunaticEngine body;
    body.StartEngine();
    return 0;
}
