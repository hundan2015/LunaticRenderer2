#pragma once
#include <algorithm>
#include <format>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>
#include "EntityComponentSystem.hpp"
namespace LunaticEngine {
class EntityManager {


   public:
    std::shared_ptr<float> deltaTime;
    std::map<std::string, std::shared_ptr<System>> systemList;
    std::set<std::shared_ptr<Entity>> entityList;
    std::queue<std::shared_ptr<Entity>> needToDestroy;
    void registerToSystem(std::shared_ptr<Entity> entity) {
        bool haveASystem = false;
        for (auto &system : systemList) {
            auto &systemMap = system.second->mRequiredComponents;
            std::set<std::string> entityHave;
            for (auto &component : entity->mComponents) {
                std::cout << "Entity have:" << component.first << std::endl;
                entityHave.insert(component.first);
            }
            for (auto &component : systemMap) {
                std::cout << "System need:" << component << std::endl;
            }
            std::vector<std::string> res;
            std::ranges::set_difference(entityHave.begin(), entityHave.end(),
                                        systemMap.begin(), systemMap.end(),
                                        std::back_inserter(res));
            if (res.empty()) {
                system.second->registerToSystem(entity);
                std::cout << "Entity " << entity->getName()
                          << " has registered to " << system.second->kName
                          << std::endl;
                haveASystem = true;
            }
        }
        if (!haveASystem) {
            needToDestroy.push(entity);
        }
    }
    void registerSystem(std::shared_ptr<System> systemPtr) {
        auto iter = systemList.find(systemPtr->kName);
        if (iter == systemList.end()) {
            systemList.insert(std::make_pair(systemPtr->kName, systemPtr));
        } else {
            std::string errmsg =
                std::format("System {} has been registered.", systemPtr->kName);
            std::cout << errmsg << std::endl;
        }
    }
    // At end of loop
    void manageEntity() {
        for (auto &entity : entityList) {
            if (entity->isDirty) {
                registerToSystem(entity);
            }
        }
        while (!needToDestroy.empty()) {
            auto entityPtr = needToDestroy.front();
            auto iter = entityList.find(entityPtr);
            if (iter == entityList.end()) {
                std::cout << "Shit!" << std::endl;
            }
            std::cout << entityList.size() << std::endl;
            entityList.erase(entityPtr);
            std::cout << entityList.size() << std::endl;
            needToDestroy.pop();
        }
    }
    void destroy(std::shared_ptr<Entity> entity) { needToDestroy.push(entity); }
    void destroy(std::shared_ptr<Component> component) {}
    void logicalTick() {
        for (auto &system : systemList) {
            system.second->onTick(*deltaTime);
        }
        manageEntity();
    }
};
// std::map<std::string, std::>
}  // namespace LunaticEngine