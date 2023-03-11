#include <iostream>
#include <memory>
#include "../core/ResourceCore.h"
#include "Systems/RenderSystem.hpp"

int main() {
    std::shared_ptr<lunatic_engine::RenderingCore> rendering_core =
        std::make_shared<lunatic_engine::RenderingCore>();
    std::shared_ptr<lunatic_engine::ResourceCore> resource_core =
        std::make_shared<lunatic_engine::ResourceCore>();
    resource_core->rendering_core = rendering_core;
    lunatic_engine::RenderingSystem rendering_system(rendering_core);
    lunatic_engine::CameraSystem camera_system;
    auto render_command_test = []() { std::cout << "Ticking!" << std::endl; };
    rendering_core->InitOpenGL();
    std::thread tick_thread([&]() {
        // rendering_core->InsertRenderCommandGroup(render_command_test);
        {
            float vertices[] = {
                0.5f,  0.5f,  0.0f,  // top right
                0.5f,  -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f, 0.5f,  0.0f   // top left
            };
            unsigned int indices[] = {
                // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            unsigned int VBO, VAO, EBO;
            bool isOK = false;
            auto vao_create = [&]() {
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
                glGenBuffers(1, &EBO);
                // bind the Vertex Array Object first, then bind and set vertex
                // buffer(s), and then configure vertex attributes(s).
                glBindVertexArray(VAO);

                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                             GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                             GL_STATIC_DRAW);

                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                                      3 * sizeof(float), (void *)0);
                glEnableVertexAttribArray(0);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
                isOK = true;
            };
            rendering_core->InsertResoureCommandGroup(vao_create);
            while (!isOK)
                ;

            // Make a mesh component.
            std::shared_ptr<lunatic_engine::MeshContent> mesh_content_ptr =
                std::make_shared<lunatic_engine::MeshContent>();
            mesh_content_ptr->triangle_count = 2;
            mesh_content_ptr->vao = VAO;
            std::shared_ptr<lunatic_engine::Mesh> mesh_ptr =
                std::make_shared<lunatic_engine::Mesh>();
            mesh_ptr->mesh_content = mesh_content_ptr;

            // Make a shader component.
            std::shared_ptr<lunatic_engine::ShaderContent> shader_content_ptr =
                resource_core->GetShaderContent(
                    "D:\\Programing\\c++\\CmakeProjects\\TestProject\\lunatic_"
                    "engine\\assets\\Shader\\TriangleShader_vs.glsl",
                    "D:\\Programing\\c++\\CmakeProjects\\TestProject\\lunatic_"
                    "engine\\assets\\Shader\\TriangleShader_fs.glsl");
            std::cout << "ShaderProgram" << shader_content_ptr->shader_program
                      << std::endl;

            std::shared_ptr<lunatic_engine::Material> material_ptr =
                std::make_shared<lunatic_engine::Material>();
            material_ptr->shader_content = shader_content_ptr;

            std::shared_ptr<lunatic_engine::Transform> transform_ptr =
                std::make_shared<lunatic_engine::Transform>();
            transform_ptr->scale_x = 1;
            transform_ptr->scale_y = 1;
            transform_ptr->scale_z = 1;

            std::shared_ptr<lunatic_engine::Entity> entity =
                std::make_shared<lunatic_engine::Entity>();
            entity->AddComponent<lunatic_engine::Material>(material_ptr);
            entity->AddComponent<lunatic_engine::Mesh>(mesh_ptr);
            entity->AddComponent<lunatic_engine::Transform>(transform_ptr);

            std::shared_ptr<lunatic_engine::MainCamera> main_camera_ptr =
                std::make_shared<lunatic_engine::MainCamera>();
            std::shared_ptr<lunatic_engine::Transform> transform_ptr2 =
                std::make_shared<lunatic_engine::Transform>();
            transform_ptr2->scale_x = 1;
            transform_ptr2->scale_y = 1;
            transform_ptr2->scale_z = 1;

            transform_ptr2->position_z = 2;
            transform_ptr2->position_y = 0.2f;
            glm::qua<float> temp(glm::radians(glm::vec3(0,0,15)));
            transform_ptr2->rotation_w = temp.w;
            transform_ptr2->rotation_x = temp.x;
            transform_ptr2->rotation_y = temp.y;
            transform_ptr2->rotation_z = temp.z;
            std::shared_ptr<lunatic_engine::Entity> camera =
                std::make_shared<lunatic_engine::Entity>();
            camera->AddComponent(main_camera_ptr);
            camera->AddComponent<lunatic_engine::Transform>(transform_ptr2);

            camera_system.RegisterToSystem(camera);

            rendering_system.RegisterToSystem(entity);
        }
    });
    while (rendering_core->IsRenderEnabled()) {
        camera_system.OnTick(1);
        rendering_system.OnTick(1);
        rendering_core->RenderTick();
        rendering_core->SwapRenderingQueue();
    }

    // Simulate the Lunatic Gaming Context.

    tick_thread.join();
    std::cout << "Rendering system test.\n";
}
