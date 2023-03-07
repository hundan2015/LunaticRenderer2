#include "RenderingCore.h"
#include <memory>

void lunatic_engine::RenderingCore::RenderTick() {
    glClearColor(0.2F, 0.3F, 0.3F, 1.0F);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    while (!command_group_queuePrev_.empty()) {
        const std::function<void()>& command_group_lambda =
            command_group_queuePrev_.front();
        command_group_lambda();
        command_group_queuePrev_.pop();
    }
    glfwSwapBuffers(window_);
    glfwPollEvents();
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
bool lunatic_engine::RenderingCore::InitOpenGL() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    window_ = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
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
    return true;
}
