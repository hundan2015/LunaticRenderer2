#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "Components/Renderer.h"
#include "GLFW/glfw3.h"
#include "Systems/RenderingManager.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
using LunaticEngine::RenderingManager;
void im_gui_render_function(GLFWwindow* window);
int main(int, char**)
{
    //RenderingManager::mRenderingManagerSingletonRef = nullptr;
    std::cout << "Hello, world?>Blade!\n";
    std::cout << "Hello, world?>Blade!\n";
    std::cout << "Hello, world?>Blade!\n";
    std::cout << "Hello, world?>Blade!\n";
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glm::vec4 vec(1, 1, 1, 1);
    glm::mat4 trans(1.0f);

    RenderingManager& renderingManager = RenderingManager::getManager();

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    trans = glm::translate(trans, glm::vec3(1, 1, 1));
    vec = trans * vec;
    glViewport(0, 0, 800, 600);
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        {
            renderingManager.insertRenderCommandGroup([]() {
                float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f,
                                    0.0f,  0.0f,  0.5f, 0.0f};
                unsigned int vbo;
                glGenBuffers(1, &vbo);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                             GL_STATIC_DRAW);
                const char* vertexShaderSource =
                    "#version 330 core\n"
                    "layout (location = 0) in vec3 aPos;\n"
                    "void main()\n"
                    "{\n"
                    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                    "}\0";
                unsigned int vertexShader;
                vertexShader = glCreateShader(GL_VERTEX_SHADER);
                glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
                glCompileShader(vertexShader);
                const char* fragmentShaderSource =
                    "#version 330 core\n"
                    "out vec4 FragColor;\n"
                    "void main() {\n"
                    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                    "}\0";
                unsigned int fragmentShader;
                fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
                glShaderSource(fragmentShader, 1, &fragmentShaderSource,
                               nullptr);
                glCompileShader(fragmentShader);

                unsigned int shaderProgram;
                shaderProgram = glCreateProgram();
                glAttachShader(shaderProgram, vertexShader);
                glAttachShader(shaderProgram, fragmentShader);
                glLinkProgram(shaderProgram);

                glUseProgram(shaderProgram);
                glDeleteProgram(vertexShader);
                glDeleteProgram(fragmentShader);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                      3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                unsigned int vao;
                glGenVertexArrays(1, &vao);

                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                             GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                      3 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(0);

                glUseProgram(shaderProgram);
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);
            });
        }
        renderingManager.renderTick();
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        { im_gui_render_function(window); }
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

void im_gui_render_function(GLFWwindow* window) {
    static bool showDebug = true;
    static float tempFloat = 0.0f;
    static int counter = 0;
    ImGui::ShowDebugLogWindow(&showDebug);
    ImGui::Begin(
        "Hello, world!", nullptr,
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove);  // Create a window called "Hello,
                                       // world!" and append into it.
    ImGui::SetWindowPos(ImVec2(0, 0));
    int windowX, windowY;

    glfwGetWindowSize(window, &windowX, &windowY);
    ImGui::SetWindowSize(
        ImVec2(static_cast<float>(windowX / 4), static_cast<float>(windowY)));

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open")) {
                ImGui::DebugLog("shit\n");
                std::cout << "stio";
            }
            if (ImGui::MenuItem("Save")) {
                std::cout << "ss";
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    ImGui::Text("This is some useful text.");  // Display some text (you can use
                                               // a format strings too)
    if (ImGui::Button("shit")) {
        counter++;
    }
    ImGui::SliderFloat("float", &tempFloat, 0.0f,
                       1.0f);  // Edit 1 float using a slider from 0.0f to 1.0f

    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}