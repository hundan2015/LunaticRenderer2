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

namespace lunatic_engine {
class EntityManager {
   public:
    // std::shared_ptr<float> deltaTime;
    std::map<std::string, std::shared_ptr<System>> mSystemList;

    std::set<std::shared_ptr<Entity>> mEntityList;
    std::queue<std::shared_ptr<Entity>> mNeedToDestroy;
    std::shared_ptr<Entity> mMainCamera;

    void registerToSystem(const std::shared_ptr<Entity> &entity) {
        bool haveASystem = false;
        for (auto &system : mSystemList) {
            auto &systemMap = system.second->mRequiredComponents;
            std::set<std::string> entityHave;
            for (auto &component : entity->mComponents) {
                std::cout << "Entity have:" << component.first << std::endl;
                entityHave.insert(component.first);
            }
            for (const auto &component : systemMap) {
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
            mNeedToDestroy.push(entity);
        }
    }

    void registerSystem(const std::shared_ptr<System> &systemPtr) {
        auto iter = mSystemList.find(systemPtr->kName);
        if (iter == mSystemList.end()) {
            mSystemList.insert(std::make_pair(systemPtr->kName, systemPtr));
        } else {
            std::string errmsg =
                std::format("System {} has been registered.", systemPtr->kName);
            std::cout << errmsg << std::endl;
        }
    }

    // At end of loop
    void manageEntity() {
        for (const auto &entity : mEntityList) {
            if (entity->mIsDirty) {
                registerToSystem(entity);
            }
        }
        while (!mNeedToDestroy.empty()) {
            auto entityPtr = mNeedToDestroy.front();
            auto iter = mEntityList.find(entityPtr);
            if (iter == mEntityList.end()) {
                std::cout << "Shit!" << std::endl;
            }
            std::cout << mEntityList.size() << std::endl;
            mEntityList.erase(entityPtr);
            std::cout << mEntityList.size() << std::endl;
            mNeedToDestroy.pop();
        }
    }

    void destroy(const std::shared_ptr<Entity> &entity) {
        mNeedToDestroy.push(entity);
    }

    void destroy(Entity &entity, const std::shared_ptr<Component> &component) {
        entity.removeComponent(component->mName);
    }

    void logicalTick(float deltaTime) {
        for (auto &system : mSystemList) {
            system.second->onTick(deltaTime);
        }
        manageEntity();
    }
};
// std::map<std::string, std::>
}  // namespace lunatic_engine