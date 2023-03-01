#include "Renderer.h"

#include <iostream>
#define warnMessageLog(x) std::cout << "\033[31m" << (x) << std::endl
void Renderer::setShaderProgram(const unsigned int& shaderProgram) {
    mShaderProgram = shaderProgram;
    mHasShaderProgram = true;
}
void Renderer::onTick() {
    if (!mHasShaderProgram) {
        std::string warningMessage = std::format(
            "Error::Renderer having VAO {} has no shader program.", mVao);
        warnMessageLog(warningMessage);
        return;
    }
    auto renderingCommand = [this] {
        glUseProgram(mShaderProgram);
        glBindVertexArray(mVao);
        // TODO: This section should use the EBO to have a smarter rendering
        // effect.
        glDrawArrays(GL_TRIANGLES, 0, mTriangleCount);
    };
    RenderingManager& renderingManager = RenderingManager::getManager();
    renderingManager.insertRenderCommandGroup(renderingCommand);
}
Renderer::Renderer(unsigned int vao, unsigned int shaderProgram)
    : mVao(vao), mShaderProgram(shaderProgram) {
    mHasShaderProgram = false;
}
Renderer::Renderer(unsigned int vao) : mVao(vao) {}
void Renderer::onStart(){};