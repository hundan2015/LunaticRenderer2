#include <iostream>
#include <memory>
#include "Core/EntityManager.hpp"
#include "../lunatic_engine/function/Core/System.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/TestSystem.h"
#include "Core/Entity.hpp"
#include "Components/Mesh.hpp"
using namespace lunatic_engine;
int main() {
    std::shared_ptr<EntityManager> entity_manager;
    std::shared_ptr<Entity> entity1 = std::make_shared<Entity>();
    std::shared_ptr<Entity> entity2 = std::make_shared<Entity>();
    entity1->addComponent<Transform>(std::make_shared<Transform>());
    entity1->is_dirty = true;
    entity1->addComponent<Mesh>(std::make_shared<Mesh>());
    entity2->addComponent<Transform>(std::make_shared<Transform>());
    entity_manager->RegisterSystem(std::make_shared<TestSystem>());
    entity_manager->RegisterSystem(std::make_shared<RenderingSystem>());
    // entity_manager.entityList.insert(entity);
    entity_manager->RegisterToSystem(entity1);
    // entity_manager.ManageEntity();
    // entity_manager.deltaTime = std::make_shared<float>(1);
    entity_manager->LogicalTick(1);
}