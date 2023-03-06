#include "LunaticEngineBody.h"

#include <barrier>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#include "../Components/TestSystem.h"
#include "RenderingManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

using std::barrier;
void lunatic_engine::LunaticEngineBody::startEngine() const {
    //  Fuck a main loop.
    auto endBar = []() noexcept {
#ifdef DEBUG_BARRIER
        std::cout << "Frame Barrier reached." << std::endl;
#endif
    };
    const float timeLast = static_cast<float>(glfwGetTime());
    bool isEngineShit = true;
    constexpr int ENGINE_THREAD_COUNT = 2;
    barrier barEnd(ENGINE_THREAD_COUNT, endBar);
    /**
     * @brief HACK:Here is a super embarrassment shit from barrier in cpp20.
     * This section need a barrier to sync three thread. But the barrier can't
     * be used as a class member. So I use two decoration lambda and two while
     * loop to use the barrier. Therefore the barrier can't be accessed by the
     * function outside.
     */
    auto logicLoopDeco = [&]() {
        while (isEngineShit) {
            const auto timeNow = static_cast<float>(glfwGetTime());
            const float deltaTime = (timeNow - timeLast) / 60;
            // logicLoop();
            mEntityManager_->logicalTick(deltaTime);
            barEnd.arrive_and_wait();
        }
    };

    auto renderLoopDeco = [&]() {
        // RenderingManager& renderingManager = RenderingManager::getManager();
        while (isEngineShit && !glfwWindowShouldClose(mWindow_)) {
            glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderLoop();
            glfwSwapBuffers(mWindow_);
            glfwPollEvents();
            barEnd.arrive_and_wait();
#ifdef DEBUG_BARRIER
            std::cout << "Render OK\n";
#endif
            /* renderingManager.swapRenderingQueue(); */
        }
        // Using the arrive_and_drop to make sure the logicLoopDeco being
        // joined.
        barEnd.arrive_and_drop();
        isEngineShit = false;
    };

    std::thread logicThread(logicLoopDeco);
    // HACK: The openGL render thread can only run in main loop! I don't know
    // why!
    renderLoopDeco();
    // renderThread.join();
    logicThread.join();
    glfwTerminate();
}

lunatic_engine::LunaticEngineBody::LunaticEngineBody() {
    initOpenGL();
    mEntityManager_ = std::make_shared<EntityManager>();
    mEntityManager_->registerSystem(std::make_shared<TestSystem>());
    mRenderingManager_ = std::make_shared<RenderingManager>();
}

void lunatic_engine::LunaticEngineBody::renderLoop() const {
    mRenderingManager_->renderTick();
}
void lunatic_engine::LunaticEngineBody::logicLoop() {}
void lunatic_engine::LunaticEngineBody::initOpenGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    mWindow_ = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (mWindow_ == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(mWindow_);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
}
