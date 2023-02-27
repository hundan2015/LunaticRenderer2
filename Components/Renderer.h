#pragma once
#include <format>
#include <iostream>
#include <string>

#include "../Systems/RenderingManager.h"
#include "Component.h"
#include "glad/glad.h"

using namespace LunaticEngine;

class Renderer : Component {
   public:
    void onTick() override;
    void setShaderProgram(const unsigned int& shaderProgram);
    void onStart() override;
    Renderer(unsigned int vao, unsigned int shaderProgram);
    Renderer(unsigned int vao);

   private:
    unsigned int mTriangleCount;
    unsigned int mVao;
    bool mHasShaderProgram = false;
    unsigned int mShaderProgram = 0;
};