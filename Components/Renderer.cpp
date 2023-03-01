#include "Renderer.h"

#include <iostream>
#define warnMessageLog(x) std::cout << "\033[31m" << (x) << std::endl
using namespace LunaticEngine;
void LunaticEngine::Renderer::setShaderProgram(
    const unsigned int& shaderProgram) {}
void LunaticEngine::Renderer::onTick() {
    if (!mHasShaderProgram) {
        std::string warningMessage = std::format(
            "Error::Renderer having VAO {} has no shader program.", 1);
        warnMessageLog(warningMessage);
        return;
    }
    /* auto renderingCommand = [this] {
        glUseProgram(mShaderProgram);
        glBindVertexArray(mVao);
        // TODO: This section should use the EBO to have a smarter rendering
        // effect.
        glDrawArrays(GL_TRIANGLES, 0, mTriangleCount);
    }; */
    RenderingManager& renderingManager = RenderingManager::getManager();
    // renderingManager.insertRenderCommandGroup(renderingCommand);
}

// LunaticEngine::Renderer::Renderer(unsigned int vao) : mVao(vao) {}
void LunaticEngine::Renderer::onStart(){};
std::string LunaticEngine::Renderer::toString() {
    std::string renderString = std::format("shit{}", 1);
    return renderString;
}