#include <iostream>
#include <memory>
#include "../lunatic_engine/function/Components/EntityComponentSystem.hpp"
#include "../lunatic_engine/function/Components/EntityManager.hpp"
#include "../lunatic_engine/function/Components/RenderSystem.hpp"
#include "../lunatic_engine/function/Components/TestSystem.h"
using namespace lunatic_engine;
int main() {
    std::shared_ptr<EntityManager> entityManager;
    std::shared_ptr<Entity> entity1 = std::make_shared<Entity>();
    std::shared_ptr<Entity> entity2 = std::make_shared<Entity>();
    entity1->addComponent<Transform>(std::make_shared<Transform>());
    entity1->is_dirty = true;
    entity1->addComponent<Mesh>(std::make_shared<Mesh>());
    entity2->addComponent<Transform>(std::make_shared<Transform>());
    entityManager->RegisterSystem(std::make_shared<TestSystem>());
    entityManager->RegisterSystem(std::make_shared<RenderingSystem>());
    // entityManager.entityList.insert(entity);
    entityManager->RegisterToSystem(entity1);
    // entityManager.ManageEntity();
    // entityManager.deltaTime = std::make_shared<float>(1);
    entityManager->LogicalTick(1);
}