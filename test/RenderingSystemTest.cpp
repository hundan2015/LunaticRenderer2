#include <iostream>
#include <memory>
#include "../core/ResourceCore.h"
#include "Systems/RenderSystem.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    lunatic_engine::model_loader::AssimpLoader assimp_loader;
    assimp_loader.LoadModel(
        "D:\\Programing\\c++\\CmakeProjects\\TestProject\\lunatic_"
        "engine\\assets\\Models\\TestModel.fbx");

    std::thread tick_thread([&]() {
        // rendering_core->InsertRenderCommandGroup(render_command_test);
        {
            // Create a texture.
            unsigned int texture;
            bool is_texture_OK = false;
            auto get_texture_content = [&]() {
                int width, height, nr_channel;
                unsigned char *data = stbi_load(
                    "D:\\Programing\\c++\\CmakeProjects\\TestProject\\lunatic_"
                    "engine\\assets\\Textures\\table.jpg",
                    &width, &height, &nr_channel, 0);
                auto texture_plus = texture;
                // Don't try to get texture's address!
                glGenTextures(1, &texture_plus);
                glBindTexture(GL_TEXTURE_2D, texture_plus);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                             GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                stbi_image_free(data);
                is_texture_OK = true;
            };
            rendering_core->InsertResoureCommandGroup(get_texture_content);
            while (is_texture_OK)
                ;

            // Make a mesh component.
            lunatic_engine::MeshContent mesh_content =
                resource_core->GetMeshContent(assimp_loader.GetMeshes()[0]);
            std::shared_ptr<lunatic_engine::MeshContent> mesh_content_ptr =
                std::make_shared<lunatic_engine::MeshContent>(mesh_content);
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

            // Make a Material component.
            std::shared_ptr<lunatic_engine::Material> material_ptr =
                std::make_shared<lunatic_engine::Material>();
            material_ptr->shader_content = shader_content_ptr;
            material_ptr->name_image_content_map.insert(std::make_pair(
                "_Albedo",
                std::make_shared<lunatic_engine::ImageContent>(texture)));

            // Make a Transform ptr.
            std::shared_ptr<lunatic_engine::Transform> transform_ptr =
                std::make_shared<lunatic_engine::Transform>();
            transform_ptr->scale_x = 1;
            transform_ptr->scale_y = 1;
            transform_ptr->scale_z = 1;
            // Make a empty entity.
            std::shared_ptr<lunatic_engine::Entity> entity =
                std::make_shared<lunatic_engine::Entity>();
            entity->AddComponent<lunatic_engine::Material>(material_ptr);
            entity->AddComponent<lunatic_engine::Mesh>(mesh_ptr);
            entity->AddComponent<lunatic_engine::Transform>(transform_ptr);

            // Make a main camera entity.
            std::shared_ptr<lunatic_engine::MainCamera> main_camera_ptr =
                std::make_shared<lunatic_engine::MainCamera>();
            std::shared_ptr<lunatic_engine::Transform> transform_ptr2 =
                std::make_shared<lunatic_engine::Transform>();
            transform_ptr2->scale_x = 1;
            transform_ptr2->scale_y = 1;
            transform_ptr2->scale_z = 1;

            transform_ptr2->position_z = 30;
            transform_ptr2->position_x = 10;
            transform_ptr2->position_y = 0;

            glm::qua<float> temp(glm::radians(glm::vec3(0, 0, 0)));
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
    // Simulate the Lunatic Gaming Context.
    while (rendering_core->IsRenderEnabled()) {
        camera_system.OnTick(1);
        rendering_system.OnTick(1);
        rendering_core->RenderTick();
        rendering_core->SwapRenderingQueue();
    }

    tick_thread.join();
    std::cout << "Rendering system test.\n";
}
