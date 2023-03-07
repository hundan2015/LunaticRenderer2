#include "LunaticEngine.h"

#include <barrier>

#include <memory>
#include <thread>

#include "../function/Systems/TestSystem.h"
#include "RenderingCore.h"

using std::barrier;
void lunatic_engine::LunaticEngine::StartEngine() const {
    //  Fuck a main loop.
    auto endbar = []() noexcept {
#ifdef DEBUG_BARRIER
        std::cout << "Frame Barrier reached." << std::endl;
#endif
    };
    auto time_last = static_cast<float>(glfwGetTime());
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
        while (is_engine_shit && rendering_core_->IsRenderEnabled()) {
            renderLoop();
            bar_end.arrive_and_wait();
#ifdef DEBUG_BARRIER
            std::cout << "Render OK\n";
#endif
            rendering_core_->SwapRenderingQueue();
        }
        // Using the arrive_and_drop to make sure the logic_loop_deco being
        // joined.
        bar_end.arrive_and_drop();
        is_engine_shit = false;
    };

    std::thread logic_thread(logic_loop_deco);
    // HACK: The openGL render thread can only run in main loop! I don't know
    // why!
    render_loop_deco();
    // renderThread.join();
    logic_thread.join();
    glfwTerminate();
}

lunatic_engine::LunaticEngine::LunaticEngine() {
    rendering_core_->InitOpenGL();
    entity_manager_ = std::make_shared<EntityManager>();
    entity_manager_->RegisterSystem(std::make_shared<TestSystem>());
    rendering_core_ = std::make_shared<RenderingCore>();
}

void lunatic_engine::LunaticEngine::renderLoop() const {
    rendering_core_->RenderTick();
}

