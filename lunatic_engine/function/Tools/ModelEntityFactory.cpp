#include "ModelEntityFactory.h"

namespace lunatic_engine {
std::shared_ptr<Entity> lunatic_engine::ModelEntityFactory::GetModelEntity(
    std::shared_ptr<model_loader::MeshNode> mesh_node) {
    std::shared_ptr<Entity> entity_root = std::make_shared<Entity>();
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    entity_root->AddComponent<Transform>(transform);
    if (mesh_node->mesh) {
        std::cout << "Have mesh!" << mesh_node->mesh->triangle_count << std::endl;
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
        lunatic_engine::MeshContent mesh_content =
            resource_core->GetMeshContent(*(mesh_node->mesh));
        mesh->mesh_content = std::make_shared<MeshContent>(mesh_content);
        entity_root->AddComponent<Mesh>(mesh);

        std::shared_ptr<lunatic_engine::ShaderContent> shader_content_ptr =
            resource_core->GetShaderContent(
                "assets/Shader/TriangleShader_vs.glsl",
                "assets/Shader/TriangleShader_fs.glsl");
        std::cout << "ShaderProgram" << shader_content_ptr->shader_program
                  << std::endl;

        // Make a Material component.
        lunatic_engine::ImageContent temp_image_content =
            resource_core->GetImageContent("assets/Textures/Chess.jpg");
        std::shared_ptr<lunatic_engine::Material> material_ptr =
            std::make_shared<lunatic_engine::Material>();
        material_ptr->shader_content = shader_content_ptr;
        material_ptr->name_image_content_map.insert(std::make_pair(
            "_Albedo", std::make_shared<lunatic_engine::ImageContent>(
                           temp_image_content)));

        entity_root->AddComponent<Material>(material_ptr);
    }
    for (auto i : mesh_node->child) {
        auto entity_child = GetModelEntity(i);
        entity_child->parent = entity_root;
        entity_root->child.emplace_back(entity_child);
    }
    return entity_root;
}
}  // namespace lunatic_engine
