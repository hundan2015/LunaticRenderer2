#pragma once

#include <functional>
#include <thread>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "RenderingManager.h"

namespace LunaticEngine {
class LunaticEngineBody {
    // std::thread& mLogicThread;
    // std::thread& mRenderingThread;
    void renderLoop();
    void logicLoop();
    void initOpenGL();
    RenderingManager& mRenderingManager;
    GLFWwindow* mWindow;

   public:
    LunaticEngineBody(GLFWwindow* window);
    void startEngine();
};
}  // namespace LunaticEngine
