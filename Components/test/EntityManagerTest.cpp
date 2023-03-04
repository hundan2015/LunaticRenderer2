#include "../EntityComponentSystem.hpp"
#include "../EntityManager.hpp"
#include <iostream>
#include <memory>
#include "../TestSystem.h"
#include "../RenderSystem.hpp"
using namespace LunaticEngine;
int main() {
    EntityManager entityManager;
    std::shared_ptr<Entity> entity1 = std::make_shared<Entity>();
    std::shared_ptr<Entity> entity2 = std::make_shared<Entity>();
    entity1->addComponent<Transform>(std::make_shared<Transform>());
    entity1->isDirty = true;
    entity1->addComponent<Mesh>(std::make_shared<Mesh>());
    entity2->addComponent<Transform>(std::make_shared<Transform>());
    entityManager.registerSystem(std::make_shared<TestSystem>());
    entityManager.registerSystem(std::make_shared<RenderingSystem>());
    // entityManager.entityList.insert(entity);
    entityManager.registerToSystem(entity1);
    // entityManager.manageEntity();
    entityManager.deltaTime = std::make_shared<float>(1);
    entityManager.logicalTick();
}