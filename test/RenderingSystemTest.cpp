#include <barrier>
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
            lunatic_engine::RegistryStation* registry_station =
                lunatic_engine::RegistryStation::GetRegistryStation();
            lunatic_engine::ModelEntityFactory model_entity_factory;
            model_entity_factory.resource_core = resource_core;
            /**
             * Create entity.
             */
            std::shared_ptr<lunatic_engine::ShaderContent> shader_content_ptr =
                resource_core->GetShaderContent(
                    "assets/Shader/TriangleShader_vs.glsl",
                    "assets/Shader/TriangleShader_fs.glsl", true);
            // TODO: The Resource core should not only get the resource itself,
            // but should also try to manage the resource.
            std::cout << "ShaderProgram" << shader_content_ptr->shader_program
                      << std::endl;
            auto temp_image_content = resource_core->GetImageContent(
                "assets/Textures/Chess.jpg", true);

            std::shared_ptr<lunatic_engine::model_loader::MeshNode> mesh_node =
                assimp_loader.GetMeshNode("assets/Models/nanosuit.obj");
            auto mesh_info =
                resource_core->GetMeshInfo("assets/Models/nanosuit.obj", true);
            // TODO: The model entity has no infomation! Need to add some
            // mesh_dir,shader_dir,and image_dir
            auto entity = model_entity_factory.GetModelEntity(
                mesh_info->root, shader_content_ptr, temp_image_content, 0);
            /**
             * Finished create entity.
             */
            lunatic_engine::EntityMeta entity_meta_pre =
                registry_station->GetEntityMeta(entity);
            json entity_json_pre = entity_meta_pre;
            std::cout << entity_json_pre << std::endl;

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

            json entity_json;
            std::ifstream json_file;
            json_file.open("shit.json");
            // json_file << entity_json;
            std::stringstream json_info;
            json_info << json_file.rdbuf();
            std::string json_res = json_info.str();
            entity_json = json::parse(json_res);

            std::cout << entity_json << std::endl;
            lunatic_engine::EntityMeta entity_meta = entity_json;
            auto entity_cp = registry_station->GetEntity(entity_meta);
            entity_manager.RegisterEntitiesToSystem(entity_cp);
            // rendering_system.RegisterToSystem(entity);
        }
    });
    // Simulate the Lunatic Gaming Context.
    std::barrier bar(2, []() noexcept {});
    auto logicDeco = [&]() {
        while (rendering_core->IsRenderEnabled()) {
            bar.arrive_and_wait();
            camera_system.OnTick(1);
            rendering_system->OnTick(1);
        }
    };

    std::thread logic(logicDeco);
    // std::thread resource(resourceDeco);

    while (rendering_core->IsRenderEnabled()) {
        // TODO: Make a lunatic engine.

        rendering_core->RenderTick();
        rendering_core->SwapRenderingQueue();

        bar.arrive_and_wait();
    }

    tick_thread.join();
    // resource.join();
    logic.join();
    std::cout << "Rendering system test.\n";
}
