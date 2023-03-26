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
    rendering_system->resource_core = resource_core;
    entity_manager.RegisterSystem(rendering_system);
    lunatic_engine::CameraSystem camera_system;
    auto render_command_test = []() { std::cout << "Ticking!" << std::endl; };
    rendering_core->InitOpenGL();

    lunatic_engine::model_loader::AssimpLoader assimp_loader;

    std::thread tick_thread([&]() {
        // rendering_core->InsertRenderCommandGroup(render_command_test);
        {
            lunatic_engine::ModelEntityFactory model_entity_factory;
            model_entity_factory.resource_core = resource_core;

            std::shared_ptr<lunatic_engine::ShaderContent> shader_content_ptr =
                resource_core->GetShaderContent(
                    "assets/Shader/TriangleShader_vs.glsl",
                    "assets/Shader/TriangleShader_fs.glsl");
            // TODO: The Resource core should not only get the resource itself,
            // but should also try to manage the resource.
            std::cout << "ShaderProgram" << shader_content_ptr->shader_program
                      << std::endl;
            lunatic_engine::ImageContent temp_image_content =
                resource_core->GetImageContent("assets/Textures/Chess.jpg");

            /*std::shared_ptr<lunatic_engine::model_loader::MeshNode> mesh_node
               = assimp_loader.GetMeshNode("assets/Models/nanosuit.obj");*/
            auto mesh_info =
                resource_core->GetMeshInfo("assets/Models/nanosuit.obj");
            // TODO: The model entity has no infomation! Need to add some
            // mesh_dir,shader_dir,and image_dir
            auto entity = model_entity_factory.GetModelEntity(
                mesh_info->root, shader_content_ptr,
                std::make_shared<lunatic_engine::ImageContent>(
                    temp_image_content),
                0);

            // mesh_node.reset();
            //  Make a main camera entity.
            std::shared_ptr<lunatic_engine::MainCamera> main_camera_ptr =
                std::make_shared<lunatic_engine::MainCamera>();
            std::shared_ptr<lunatic_engine::Transform> transform_ptr2 =
                std::make_shared<lunatic_engine::Transform>();
            transform_ptr2->scale_x = 1;
            transform_ptr2->scale_y = 1;
            transform_ptr2->scale_z = 1;

            transform_ptr2->position_z = 15;
            transform_ptr2->position_x = 0;
            transform_ptr2->position_y = 5;

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

            lunatic_engine::RegistryStation* registry_station =
                lunatic_engine::RegistryStation::GetRegistryStation();
            lunatic_engine::EntityMeta entity_meta =
                registry_station->GetEntityMeta(entity);
            json entity_json = entity_meta;
            std::cout << entity_json << std::endl;
            auto entity_cp = registry_station->GetEntity(entity_meta);
            entity_manager.RegisterEntitiesToSystem(entity_cp);
            // rendering_system.RegisterToSystem(entity);
        }
    });
    // Simulate the Lunatic Gaming Context.
    while (rendering_core->IsRenderEnabled()) {
        // TODO: Make a lunatic engine.
        camera_system.OnTick(1);
        rendering_system->OnTick(1);
        rendering_core->RenderTick();
        rendering_core->SwapRenderingQueue();
    }

    tick_thread.join();
    std::cout << "Rendering system test.\n";
}
