#include "RenderingCore.h"
#include <memory>

void lunatic_engine::RenderingCore::RenderTick() {
    openGL_lock_.lock();
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    openGL_lock_.unlock();
    ResourceTick();
    command_prev_lock_.lock();
    while (!command_group_queuePrev_.empty()) {
        ResourceTick();
        const std::function<void()>& command_group_lambda =
            command_group_queuePrev_.front();
        openGL_lock_.lock();
        command_group_lambda();
        openGL_lock_.unlock();
        command_group_queuePrev_.pop();
    }
    command_prev_lock_.unlock();
    glfwSwapBuffers(window_);
    glfwPollEvents();
}
void lunatic_engine::RenderingCore::ResourceTick() {
    resource_command_lock_.lock();
    while (!resource_command_group_queue_.empty()) {
        std::cout << "Have a resource request!\n";

        const std::function<void()>& resource_command_group_lambda =
            resource_command_group_queue_.front();
        openGL_lock_.lock();
        resource_command_group_lambda();
        openGL_lock_.unlock();
        resource_command_group_queue_.pop();
    }
    resource_command_lock_.unlock();
}
void lunatic_engine::RenderingCore::InsertRenderCommandGroup(
    const std::function<void()>& command_group_lambda) {
    command_lock_.lock();
    command_group_queue_.push(command_group_lambda);
    command_lock_.unlock();
}

std::shared_ptr<lunatic_engine::RenderingCore>
    lunatic_engine::RenderingCore::rendering_manager_singleton_ref_ = nullptr;
bool lunatic_engine::RenderingCore::InitOpenGL() {
    openGL_lock_.lock();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window_ = glfwCreateWindow(800, 600, "Lunatic Engine 2", nullptr, nullptr);
    if (window_ == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window_);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    openGL_lock_.unlock();
    return true;
}
bool lunatic_engine::RenderingCore::IsRenderEnabled() const {
    return glfwWindowShouldClose(window_) == 0;
}
void lunatic_engine::RenderingCore::InsertResoureCommandGroup(
    const std::function<void()>& resource_command_group_lambda) {
    static std::mutex locker;
    locker.lock();
    resource_command_group_queue_.push(resource_command_group_lambda);
    locker.unlock();
}
void lunatic_engine::RenderingCore::SwapRenderingQueue() {
    // Using move trying to make it faster.
    command_group_queuePrev_ = std::move(command_group_queue_);
}
