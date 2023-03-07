#pragma once

#include <functional>
#include <memory>
#include <thread>
// The glad must be front of the GLFW.
#include "glad/glad.h"

#include "../function/Components/EntityManager.hpp"
#include "GLFW/glfw3.h"
#include "RenderingCore.h"

namespace lunatic_engine {
class LunaticEngine {
    void renderLoop() const;
    static void LogicLoop();
    void InitOpenGL();
    // RenderingCore& mRenderingManager;
    std::shared_ptr<RenderingCore> rendering_manager_;
    std::shared_ptr<EntityManager> entity_manager_;
    GLFWwindow* window_{};

    // std::shared_ptr<Entity> mainCamera_;

   public:
    LunaticEngine();
    void StartEngine() const;
};
}  // namespace lunatic_engine
