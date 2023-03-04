#pragma once

#include <functional>
#include <memory>
#include <thread>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "RenderingManager.h"
#include "../Components/EntityManager.hpp"
namespace LunaticEngine {
class LunaticEngineBody {
    void renderLoop();
    void logicLoop();
    void initOpenGL();
    // RenderingManager& mRenderingManager;
    std::shared_ptr<RenderingManager> mRenderingManager;
    std::shared_ptr<EntityManager> mEntityManager;
    GLFWwindow* mWindow;

   public:
    LunaticEngineBody();
    void startEngine();
};
}  // namespace LunaticEngine
