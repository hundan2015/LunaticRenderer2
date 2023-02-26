#pragma once
#include <format>
#include <iostream>
#include <string>

#include "../Systems/RenderingManager.h"
#include "Component.h"
//#include "GLFW/glfw3.h"
#include "glad/glad.h"

using namespace LunaticEngine;

class Renderer : Component {
   public:
       void onTick() override;
       void setShaderProgram(const unsigned int& shaderProgram);

   private:
    unsigned int mTriangleCount;
    unsigned int mVao;
    bool mHasShaderProgram = false;
    unsigned int mShaderProgram = 0;
};