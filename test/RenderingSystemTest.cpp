#include <iostream>
#include <memory>
#include <thread>
// #include "../core/ResourceCore.h"
// #include "Systems/RenderSystem.hpp"
#include "Core/EntityManager.hpp"
#include "Tools/ModelEntityFactory.h"

int main() {
    std::shared_ptr<lunatic_engine::RenderingCore> rendering_core =
        std::make_shared<lunatic_engine::RenderingCore>();
    std::shared_ptr<lunatic_engine::ResourceCore> resource_core =
        std::make_shared<lunatic_engine::ResourceCore>();
    resource_core->rendering_core = rendering_core;
    lunatic_engine::EntityManager entity_manager;

    std::shared_ptr<lunatic_engine::RenderingSystem> rendering_system =
        std::make_shared<lunatic_engine::RenderingSystem>(rendering_core);
    entity_manager.RegisterSystem(rendering_system);
    lunatic_engine::CameraSystem camera_system;
    auto render_command_test = []() { std::cout << "Ticking!" << std::endl; };
    rendering_core->InitOpenGL();

    lunatic_engine::model_loader::AssimpLoader assimp_loader;
    // assimp_loader.LoadModel("assets/Models/TestModel.fbx");

    std::thread tick_thread([&]() {
        // rendering_core->InsertRenderCommandGroup(render_command_test);
        {
            /*// Create a texture.
            lunatic_engine::ImageContent temp_image_content =
                resource_core->GetImageContent("assets/Textures/table.jpg");
            // Make a mesh component.
            std::shared_ptr<lunatic_engine::model_loader::MeshNode> mesh_node =
                assimp_loader.GetMeshNode("assets/Models/TestModel.fbx");
            assert(mesh_node->mesh != nullptr);
            lunatic_engine::MeshContent mesh_content =
                resource_core->GetMeshContent(*(mesh_node->mesh));
            std::shared_ptr<lunatic_engine::MeshContent> mesh_content_ptr =
                std::make_shared<lunatic_engine::MeshContent>(mesh_content);
            std::shared_ptr<lunatic_engine::Mesh> mesh_ptr =
                std::make_shared<lunatic_engine::Mesh>();
            mesh_ptr->mesh_content = mesh_content_ptr;

            // Make a shader component.
            std::shared_ptr<lunatic_engine::ShaderContent> shader_content_ptr =
                resource_core->GetShaderContent(
                    "assets/Shader/TriangleShader_vs.glsl",
                    "assets/Shader/TriangleShader_fs.glsl");
            std::cout << "ShaderProgram" << shader_content_ptr->shader_program
                      << std::endl;

            // Make a Material component.
            std::shared_ptr<lunatic_engine::Material> material_ptr =
                std::make_shared<lunatic_engine::Material>();
            material_ptr->shader_content = shader_content_ptr;
            material_ptr->name_image_content_map.insert(std::make_pair(
                "_Albedo", std::make_shared<lunatic_engine::ImageContent>(
                               temp_image_content)));

            // material_ptr->InitMaterial();

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
            entity->AddComponent<lunatic_engine::Transform>(transform_ptr);*/

            lunatic_engine::ModelEntityFactory model_entity_factory;
            model_entity_factory.resource_core = resource_core;
            std::shared_ptr<lunatic_engine::model_loader::MeshNode> mesh_node =
                assimp_loader.GetMeshNode("assets/Models/space.obj");
            auto entity = model_entity_factory.GetModelEntity(mesh_node);

            // Make a main camera entity.
            std::shared_ptr<lunatic_engine::MainCamera> main_camera_ptr =
                std::make_shared<lunatic_engine::MainCamera>();
            std::shared_ptr<lunatic_engine::Transform> transform_ptr2 =
                std::make_shared<lunatic_engine::Transform>();
            transform_ptr2->scale_x = 1;
            transform_ptr2->scale_y = 1;
            transform_ptr2->scale_z = 1;

            transform_ptr2->position_z = 15;
            transform_ptr2->position_x = 0;
            transform_ptr2->position_y = 10;

            glm::qua<float> temp(glm::radians(glm::vec3(-45, 0, 0)));
            transform_ptr2->rotation_w = temp.w;
            transform_ptr2->rotation_x = temp.x;
            transform_ptr2->rotation_y = temp.y;
            transform_ptr2->rotation_z = temp.z;
            std::shared_ptr<lunatic_engine::Entity> camera =
                std::make_shared<lunatic_engine::Entity>();
            camera->AddComponent(main_camera_ptr);
            camera->AddComponent<lunatic_engine::Transform>(transform_ptr2);

            camera_system.RegisterToSystem(camera);
            entity_manager.RegisterEntitiesToSystem(entity);
            // rendering_system.RegisterToSystem(entity);
        }
    });
    // Simulate the Lunatic Gaming Context.
    while (rendering_core->IsRenderEnabled()) {
        camera_system.OnTick(1);
        rendering_system->OnTick(1);
        rendering_core->RenderTick();
        rendering_core->SwapRenderingQueue();
    }

    tick_thread.join();
    std::cout << "Rendering system test.\n";
}
