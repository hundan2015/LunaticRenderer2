#include <iostream>
#include <memory>
#include "../lunatic_engine/function/Core/System.hpp"
#include "Components/Mesh.hpp"
#include "Core/Entity.hpp"
#include "Core/EntityManager.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/TestSystem.h"
using namespace lunatic_engine;
int main() {
    std::shared_ptr<EntityManager> entity_manager;
    std::shared_ptr<Entity> entity1 = std::make_shared<Entity>();
    std::shared_ptr<Entity> entity2 = std::make_shared<Entity>();
    entity1->AddComponent<Transform>(std::make_shared<Transform>());
    entity1->is_dirty = true;
    entity1->AddComponent<Mesh>(std::make_shared<Mesh>());
    entity2->AddComponent<Transform>(std::make_shared<Transform>());
    entity_manager->RegisterSystem(std::make_shared<TestSystem>());
    entity_manager->RegisterSystem(std::make_shared<RenderingSystem>());
    // entity_manager.entityList.insert(entity);
    entity_manager->RegisterSingleEntityToSystem(entity1);
    // entity_manager.ManageEntity();
    // entity_manager.deltaTime = std::make_shared<float>(1);
    entity_manager->LogicalTick(1);
}