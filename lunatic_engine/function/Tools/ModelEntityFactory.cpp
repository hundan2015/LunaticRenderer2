#include "ModelEntityFactory.h"
#include "fmt/core.h"
namespace lunatic_engine {
std::shared_ptr<Entity> lunatic_engine::ModelEntityFactory::GetModelEntity(
    const std::shared_ptr<model_loader::MeshNode>& mesh_node,
    const std::shared_ptr<lunatic_engine::ShaderContent>& shader_content_ptr,
    const std::shared_ptr<lunatic_engine::ImageContent>& temp_image_content,
    int mesh_counter = 0) {
    std::shared_ptr<Entity> entity_root = std::make_shared<Entity>();
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    entity_root->AddComponent<Transform>(transform);
    if (mesh_node->mesh) {
        std::cout << fmt::format("Have a Mesh! The triangle count is {}\n",
                                 mesh_node->mesh->triangle_count);
        std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
        /**
         * @warning this part is super shit! First it get a mesh_content then it
         * use copy function to make a mesh content. In fact, these
         * assimpLoader::mesh content should get in the resource core, and not
         * expose to the function layer.
         */

        mesh->mesh_content =
            resource_core->GetMeshContent(mesh_node->mesh, false);
        mesh->mesh_num = mesh_counter;
        mesh_counter++;
        entity_root->AddComponent<Mesh>(mesh);
        // Shit part end.

        std::shared_ptr<lunatic_engine::Material> material_ptr =
            std::make_shared<lunatic_engine::Material>();
        material_ptr->shader_content = shader_content_ptr;
        // In this engine's default pipeline, we use _Albedo to describe the
        // diffuse map.
        material_ptr->name_image_content_map.insert(
            std::make_pair("_Albedo", temp_image_content));

        entity_root->AddComponent<Material>(material_ptr);
    }
    for (auto& mesh_node_child : mesh_node->child) {
        auto entity_child = GetModelEntity(mesh_node_child, shader_content_ptr,
                                           temp_image_content, mesh_counter);
        entity_child->parent = entity_root;
        entity_root->child.emplace_back(entity_child);
    }
    return entity_root;
}

std::shared_ptr<Entity> ModelEntityFactory::GetModelEntity(
    const std::shared_ptr<MeshContentNode>& mesh_node,
    const std::shared_ptr<lunatic_engine::ShaderContent>& shader_content_ptr,
    const std::shared_ptr<lunatic_engine::ImageContent>& temp_image_content_,
    int mesh_counter = 0) {
    std::shared_ptr<Entity> entity_root = std::make_shared<Entity>();
    std::shared_ptr<Transform> transform = std::make_shared<Transform>();
    entity_root->AddComponent<Transform>(transform);
    if (mesh_node->mesh_content) {
        auto mesh_ptr = std::make_shared<Mesh>();
        mesh_ptr->mesh_content = mesh_node->mesh_content;
        mesh_ptr->mesh_num = mesh_node->num;
        mesh_ptr->mesh_dir = mesh_node->mesh_dir;
        entity_root->AddComponent<Mesh>(mesh_ptr);
        std::shared_ptr<lunatic_engine::Material> material_ptr =
            std::make_shared<lunatic_engine::Material>();
        material_ptr->shader_fs_dir = shader_content_ptr->fragment_shader_dir;
        material_ptr->shader_vs_dir = shader_content_ptr->vertex_shader_dir;
        material_ptr->shader_content = shader_content_ptr;
        // In this engine's default pipeline, we use _Albedo to describe the
        // diffuse map.
        material_ptr->name_dir_map.insert(
            std::make_pair("_Albedo", temp_image_content_->image_dir));
        material_ptr->name_image_content_map.insert(
            std::make_pair("_Albedo", temp_image_content_));

        entity_root->AddComponent<Material>(material_ptr);
    }
    for (auto& mesh_node_child : mesh_node->children) {
        auto entity_child = GetModelEntity(mesh_node_child, shader_content_ptr,
                                           temp_image_content_, mesh_counter);
        entity_child->parent = entity_root;
        entity_root->child.emplace_back(entity_child);
    }
    return entity_root;
}
std::shared_ptr<Entity> ModelEntityFactory::GetModelEntity(
    std::string vertex_dir, std::string fragment_dir, std::string texture_dir,
    std::string mesh_dir, bool is_immediatly) {
    std::shared_ptr<lunatic_engine::ShaderContent> shader_content_ptr =
        resource_core->GetShaderContent(vertex_dir, fragment_dir,
                                        is_immediatly);
    // TODO: The Resource core should not only get the resource itself,
    // but should also try to manage the resource.
    if (is_immediatly) {
        std::cout << "ShaderProgram" << shader_content_ptr->shader_program
                  << std::endl;
    }

    auto temp_image_content =
        resource_core->GetImageContent(texture_dir, is_immediatly);

    /*std::shared_ptr<lunatic_engine::model_loader::MeshNode> mesh_node =
        assimp_loader.GetMeshNode("assets/Models/nanosuit.obj");*/
    auto mesh_info = resource_core->GetMeshInfo(mesh_dir, is_immediatly);
    // TODO: The model entity has no infomation! Need to add some
    // mesh_dir,shader_dir,and image_dir
    auto entity = GetModelEntity(mesh_info->root, shader_content_ptr,
                                 temp_image_content, 0);
    return entity;
}
}  // namespace lunatic_engine
