#pragma once

#include <functional>
#include <memory>
#include <thread>
// The glad must be front of the GLFW.
#include "glad/glad.h"

#include "../Components/EntityManager.hpp"
#include "GLFW/glfw3.h"
#include "RenderingManager.h"

namespace lunatic_engine {
class LunaticEngineBody {
    void renderLoop() const;
    static void logicLoop();
    void initOpenGL();
    // RenderingManager& mRenderingManager;
    std::shared_ptr<RenderingManager> mRenderingManager_;
    std::shared_ptr<EntityManager> mEntityManager_;
    GLFWwindow* mWindow_{};

    // std::shared_ptr<Entity> mainCamera_;

   public:
    LunaticEngineBody();
    void startEngine() const;
};
}  // namespace lunatic_engine
