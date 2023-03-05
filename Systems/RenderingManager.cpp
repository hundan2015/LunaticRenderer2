#include "RenderingManager.h"
#include <memory>

void LunaticEngine::RenderingManager::renderTick() {
    while (!mCommandGroupQueuePrev_.empty()) {
        const std::function<void()>& commandGroupLambda =
            mCommandGroupQueuePrev_.front();
        commandGroupLambda();
        mCommandGroupQueuePrev_.pop();
    }
}
void LunaticEngine::RenderingManager::insertRenderCommandGroup(
    const std::function<void()>& commandGroupLambda) {
    static std::mutex locker;
    locker.lock();
    mCommandGroupQueue_.push(commandGroupLambda);
    locker.unlock();
}
/* LunaticEngine::RenderingManager& LunaticEngine::RenderingManager::getManager() {
    if (mRenderingManagerSingletonRef_ == nullptr) {
        mRenderingManagerSingletonRef_ = std::make_shared<RenderingManager>();
    }
    return *mRenderingManagerSingletonRef_;
}
std::shared_ptr<LunaticEngine::RenderingManager>
LunaticEngine::RenderingManager::getManagerPtr() {
    if (mRenderingManagerSingletonRef_ == nullptr) {
        mRenderingManagerSingletonRef_ = std::make_shared<RenderingManager>();
    }
    return std::make_shared<RenderingManager>(mRenderingManagerSingletonRef_);
} */

std::shared_ptr<LunaticEngine::RenderingManager>
    LunaticEngine::RenderingManager::mRenderingManagerSingletonRef_ = nullptr;