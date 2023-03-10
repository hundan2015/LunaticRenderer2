#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
// glad is always front of glfw. Don't forget to separate them because of the
// clang-format!
#include "glad/glad.h"

#include "GLFW/glfw3.h"

namespace lunatic_engine {
class RenderingCore {
   public:
    RenderingCore() = default;
    GLFWwindow* window_{};
    // It's a singleton shit.
    // RenderingCore(const RenderingCore& another) = delete;
    // void operator=(const RenderingCore&) = delete;
    bool InitOpenGL();
    /*
     * @brief Return the Render is rendering.
     * WTF is [[nodiscard]]??
     */
    [[nodiscard]] bool IsRenderEnabled() const {
        return glfwWindowShouldClose(window_) == 0;
    }
    void InsertRenderCommandGroup(
        const std::function<void()>& command_group_lambda);
    void InsertResoureCommandGroup(
        const std::function<void()>& resource_command_group_lambda) {
        static std::mutex locker;
        locker.lock();
        resource_command_group_queue_.push(resource_command_group_lambda);
        locker.unlock();
    }

    /**
     * @brief RenderTick is defined to be double buffer in the Render System.
     */
    void RenderTick();

    // We only hope the RenderingCore itself have its own life cycle.
    static std::shared_ptr<RenderingCore> rendering_manager_singleton_ref_;

    void SwapRenderingQueue() {
        // Using move trying to make it faster.
        command_group_queuePrev_ = std::move(command_group_queue_);
    }

   private:
    std::mutex command_lock_;
    std::mutex command_prev_lock_;
    std::mutex resource_command_lock_;
    std::queue<std::function<void()>> command_group_queue_;
    std::queue<std::function<void()>> resource_command_group_queue_;
    std::queue<std::function<void()>> command_group_queuePrev_;
};

}  // namespace lunatic_engine
