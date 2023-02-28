#include <barrier>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

#include "LunaticEngineBody.h"
#include "RenderingManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

using std::barrier;
void LunaticEngine::LunaticEngineBody::startEngine() {
    initOpenGL();
    //  Fuck a main loop.
    auto endBar = []() noexcept {
#ifdef DEBUG_BARRIER
        std::cout << "Frame Barrier reached." << std::endl;
#endif
    };
    bool isEngineShit = true;
    const int ENGINE_THREAD_COUNT = 2;
    barrier barEnd(ENGINE_THREAD_COUNT, endBar);
    /**
     * @brief HACK:Here is a super imbarrasment shit from barrier in cpp20. This
     * section need a barrier to sync three thread. But the barrier can't be
     * used as a class member. So I use two decoration lambda and two while
     * loop to use the barrier. Therefore the barrier can't be accessed by the
     * function outside.
     */
    auto logicLoopDeco = [&]() {
        while (isEngineShit) {
            logicLoop();
            barEnd.arrive_and_wait();
        }
    };

    auto renderLoopDeco = [&]() {
        RenderingManager& renderingManager = RenderingManager::getManager();
        while (isEngineShit && !glfwWindowShouldClose(mWindow)) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderLoop();
            glfwSwapBuffers(mWindow);
            glfwPollEvents();
            barEnd.arrive_and_wait();
#ifdef DEBUG_BARRIER
            std::cout << "Render OK\n";
#endif
            renderingManager.swapRenderingQueue();

            
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

LunaticEngine::LunaticEngineBody::LunaticEngineBody(GLFWwindow* window)
    : mRenderingManager(RenderingManager::getManager()), mWindow(window) {}

void LunaticEngine::LunaticEngineBody::renderLoop() {
    mRenderingManager.renderTick();
}
void LunaticEngine::LunaticEngineBody::logicLoop() {}
void LunaticEngine::LunaticEngineBody::initOpenGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    mWindow = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (mWindow == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(mWindow);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
}
