#include "RenderingCore.h"
#include <memory>

void lunatic_engine::RenderingCore::RenderTick() {
    while (!command_group_queuePrev_.empty()) {
        const std::function<void()>& command_group_lambda =
            command_group_queuePrev_.front();
        command_group_lambda();
        command_group_queuePrev_.pop();
    }
}
void lunatic_engine::RenderingCore::InsertRenderCommandGroup(
    const std::function<void()>& command_group_lambda) {
    static std::mutex locker;
    locker.lock();
    command_group_queue_.push(command_group_lambda);
    locker.unlock();
}


std::shared_ptr<lunatic_engine::RenderingCore>
    lunatic_engine::RenderingCore::rendering_manager_singleton_ref_ = nullptr;