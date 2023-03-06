#include "RenderingManager.h"
#include <memory>

void lunatic_engine::RenderingManager::renderTick() {
    while (!mCommandGroupQueuePrev_.empty()) {
        const std::function<void()>& commandGroupLambda =
            mCommandGroupQueuePrev_.front();
        commandGroupLambda();
        mCommandGroupQueuePrev_.pop();
    }
}
void lunatic_engine::RenderingManager::insertRenderCommandGroup(
    const std::function<void()>& commandGroupLambda) {
    static std::mutex locker;
    locker.lock();
    mCommandGroupQueue_.push(commandGroupLambda);
    locker.unlock();
}
/* lunatic_engine::RenderingManager& lunatic_engine::RenderingManager::getManager() {
    if (mRenderingManagerSingletonRef_ == nullptr) {
        mRenderingManagerSingletonRef_ = std::make_shared<RenderingManager>();
    }
    return *mRenderingManagerSingletonRef_;
}
std::shared_ptr<lunatic_engine::RenderingManager>
lunatic_engine::RenderingManager::getManagerPtr() {
    if (mRenderingManagerSingletonRef_ == nullptr) {
        mRenderingManagerSingletonRef_ = std::make_shared<RenderingManager>();
    }
    return std::make_shared<RenderingManager>(mRenderingManagerSingletonRef_);
} */

std::shared_ptr<lunatic_engine::RenderingManager>
    lunatic_engine::RenderingManager::mRenderingManagerSingletonRef_ = nullptr;