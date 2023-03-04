#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>

using std::mutex;
namespace LunaticEngine {
class RenderingManager {
    /** TODO: A RenderingManager is not only the singliton but belong to a Engine
     * Context.*/
   public:
    RenderingManager() = default;
    // It's a singliton shit.
    RenderingManager(const RenderingManager& another) = delete;
    void operator=(const RenderingManager&) = delete;

    void insertRenderCommandGroup(std::function<void()> commandGroupLambda);

    /**
     * @brief RenderTick is defined to be double buffer in the Render System.
     */

    void renderTick();
    // FIXME: Rendering Manager would no longer be a Singliton!
    static RenderingManager& getManager();

    // We only hope the RenderingManager itself have its own life cycle.
    static std::unique_ptr<RenderingManager> mRenderingManagerSingletonRef;

    void swapRenderingQueue() {
        // Using move trying to make it faster.
        mCommandGroupQueuePrev = std::move(mCommandGroupQueue);
    }

   private:
    std::queue<std::function<void()>> mCommandGroupQueue;
    std::queue<std::function<void()>> mCommandGroupQueuePrev;
};

}  // namespace LunaticEngine
