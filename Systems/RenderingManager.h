#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

namespace LunaticEngine {
class RenderingManager {
    /** TODO: A RenderingManager is not only the singliton but belong to a
     * Engine Context.*/
   public:
    RenderingManager() = default;
    // It's a singliton shit.
    //RenderingManager(const RenderingManager& another) = delete;
    //void operator=(const RenderingManager&) = delete;

    void insertRenderCommandGroup(
        const std::function<void()>& commandGroupLambda);

    /**
     * @brief RenderTick is defined to be double buffer in the Render System.
     */

    void renderTick();
    // FIXME: Rendering Manager would no longer be a Singliton!
    //static RenderingManager& getManager();
    //static std::shared_ptr<RenderingManager> getManagerPtr();

    // We only hope the RenderingManager itself have its own life cycle.
    static std::shared_ptr<RenderingManager> mRenderingManagerSingletonRef_;

    void swapRenderingQueue() {
        // Using move trying to make it faster.
        mCommandGroupQueuePrev_ = std::move(mCommandGroupQueue_);
    }

   private:
    std::queue<std::function<void()>> mCommandGroupQueue_;
    std::queue<std::function<void()>> mCommandGroupQueuePrev_;
};

}  // namespace LunaticEngine
