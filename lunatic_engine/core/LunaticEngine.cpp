#include "LunaticEngine.h"

#include <barrier>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

#include "../function/Components/TestSystem.h"
#include "RenderingCore.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

using std::barrier;
void lunatic_engine::LunaticEngine::StartEngine() const {
    //  Fuck a main loop.
    auto endbar = []() noexcept {
#ifdef DEBUG_BARRIER
        std::cout << "Frame Barrier reached." << std::endl;
#endif
    };
    float time_last = static_cast<float>(glfwGetTime());
    bool is_engine_shit = true;
    constexpr int kEngineThreadCount = 2;
    barrier bar_end(kEngineThreadCount, endbar);
    /**
     * @brief HACK:Here is a super embarrassment shit from barrier in cpp20.
     * This section need a barrier to sync three thread. But the barrier can't
     * be used as a class member. So I use two decoration lambda and two while
     * loop to use the barrier. Therefore the barrier can't be accessed by the
     * function outside.
     */
    auto logic_loop_deco = [&]() {
        while (is_engine_shit) {
            const auto time_now = static_cast<float>(glfwGetTime());
            const float delta_time = (time_now - time_last) / 60;
            time_last = time_now;
            // LogicLoop();
            entity_manager_->LogicalTick(delta_time);
            bar_end.arrive_and_wait();
        }
    };

    auto render_loop_deco = [&]() {
        // RenderingCore& renderingManager = RenderingCore::getManager();
        while (is_engine_shit && !glfwWindowShouldClose(window_)) {
            glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderLoop();
            glfwSwapBuffers(window_);
            glfwPollEvents();
            bar_end.arrive_and_wait();
#ifdef DEBUG_BARRIER
            std::cout << "Render OK\n";
#endif
            /* renderingManager.SwapRenderingQueue(); */
        }
        // Using the arrive_and_drop to make sure the logic_loop_deco being
        // joined.
        bar_end.arrive_and_drop();
        is_engine_shit = false;
    };

    std::thread logicThread(logic_loop_deco);
    // HACK: The openGL render thread can only run in main loop! I don't know
    // why!
    render_loop_deco();
    // renderThread.join();
    logicThread.join();
    glfwTerminate();
}

lunatic_engine::LunaticEngine::LunaticEngine() {
    InitOpenGL();
    entity_manager_ = std::make_shared<EntityManager>();
    entity_manager_->RegisterSystem(std::make_shared<TestSystem>());
    rendering_manager_ = std::make_shared<RenderingCore>();
}

void lunatic_engine::LunaticEngine::renderLoop() const {
    rendering_manager_->RenderTick();
}
void lunatic_engine::LunaticEngine::LogicLoop() {}
void lunatic_engine::LunaticEngine::InitOpenGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window_ = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (window_ == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window_);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
}
