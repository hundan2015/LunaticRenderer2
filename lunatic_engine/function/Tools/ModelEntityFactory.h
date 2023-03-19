#pragma once
#include "../../core/AssimpLoader.h"
#include "../../core/ResourceCore.h"
#include "../Core/Entity.hpp"
#include "../Systems/RenderSystem.hpp"
namespace lunatic_engine {
class ModelEntityFactory {
   public:
    std::shared_ptr<ResourceCore> resource_core;
    std::shared_ptr<Entity> GetModelEntity(
        std::shared_ptr<model_loader::MeshNode> mesh_node,
        const std::shared_ptr<lunatic_engine::ShaderContent>& shader_content_ptr);
};
}  // namespace lunatic_engine
