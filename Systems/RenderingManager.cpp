#include "RenderingManager.h"
void LunaticEngine::RenderingManager::renderTick() {
    while (!mCommandGroupQueue.empty()) {
        const std::function<void()>& commandGroupLambda =
            mCommandGroupQueue.front();
        commandGroupLambda();
        mCommandGroupQueue.pop();
    }
}
void LunaticEngine::RenderingManager::insertRenderCommandGroup(
    std::function<void()> commandGroupLambda) {
    static std::mutex locker;
    locker.lock();
    mCommandGroupQueue.push(commandGroupLambda);
    locker.unlock();
}
LunaticEngine::RenderingManager& LunaticEngine::RenderingManager::getManager()
{
    if (mRenderingManagerSingletonRef == nullptr) {
        mRenderingManagerSingletonRef = std::unique_ptr<RenderingManager>(new RenderingManager());
    }
    return *mRenderingManagerSingletonRef;
}

std::unique_ptr<LunaticEngine::RenderingManager>
    LunaticEngine::RenderingManager::mRenderingManagerSingletonRef = nullptr;