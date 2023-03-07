#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

namespace lunatic_engine {
class RenderingCore {
    /** TODO: A RenderingCore is not only the singleton but belong to a
     * Engine Context.*/
   public:
    RenderingCore() = default;
    // It's a singleton shit.
    //RenderingCore(const RenderingCore& another) = delete;
    //void operator=(const RenderingCore&) = delete;

    void InsertRenderCommandGroup(
        const std::function<void()>& command_group_lambda);

    /**
     * @brief RenderTick is defined to be double buffer in the Render System.
     */
    void RenderTick();
    // FIXME: Rendering Manager would no longer be a Singleton!
    //static RenderingCore& getManager();
    //static std::shared_ptr<RenderingCore> getManagerPtr();

    // We only hope the RenderingCore itself have its own life cycle.
    static std::shared_ptr<RenderingCore> rendering_manager_singleton_ref_;

    void SwapRenderingQueue() {
        // Using move trying to make it faster.
        command_group_queuePrev_ = std::move(command_group_queue_);
    }

   private:
    std::queue<std::function<void()>> command_group_queue_;
    std::queue<std::function<void()>> command_group_queuePrev_;
};

}  // namespace lunatic_engine
