#pragma once

#include <fmt/core.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <queue>
#include <set>
#include <utility>
#include <vector>
#include "Component.hpp"
#include "Entity.hpp"
#include "System.hpp"
using json = nlohmann::json;
namespace lunatic_engine {
class EntityManager {
   public:
    // std::shared_ptr<float> deltaTime;
    std::map<std::string, std::shared_ptr<System>> system_list;

    std::set<std::shared_ptr<Entity>> entity_list;
    std::queue<std::shared_ptr<Entity>> need_to_destroy;
    std::shared_ptr<Entity> main_camera;
    // TODO: These should move to a new class named SystemManager.
    void RegisterEntitiesToSystem(std::shared_ptr<Entity> entity) {
        std::queue<std::shared_ptr<Entity>> entity_queue;
        entity_queue.push(entity);
        while (!entity_queue.empty()) {
            auto front = entity_queue.front();
            entity_queue.pop();
            for (const auto &child : front->child) {
                entity_queue.push(child);
            }
            RegisterSingleEntityToSystem(front, false);
        }
    }

    void RegisterSingleEntityToSystem(const std::shared_ptr<Entity> &entity,
                                      bool can_it_destroy = true) {
        bool have_a_system = false;
        /**
         * It's using the string set to compare.
         */
        for (auto &system : system_list) {
            auto &system_map = system.second->required_components;
            std::set<std::string> entity_have;
            for (auto &component : entity->components) {
                std::cout << "Entity have:" << component.first << std::endl;
                entity_have.insert(component.first);
            }
            for (const auto &component : system_map) {
                std::cout << "System need:" << component << std::endl;
            }
            std::vector<std::string> res;
            // Using the set operations.
            // TODO:In the future we should import the concept named Architype.
            std::ranges::set_difference(system_map.begin(), system_map.end(),
                                        entity_have.begin(), entity_have.end(),
                                        std::back_inserter(res));
            if (res.empty()) {
                system.second->RegisterToSystem(entity);
                std::cout << "Entity " << entity->GetName()
                          << " has registered to " << system.second->kName
                          << std::endl;
                have_a_system = true;
            }
        }
        if (!have_a_system && can_it_destroy) {
            need_to_destroy.push(entity);
        }
    }

    void RegisterSystem(const std::shared_ptr<System> &system_ptr) {
        auto iter = system_list.find(system_ptr->kName);
        if (iter == system_list.end()) {
            system_list.insert(std::make_pair(system_ptr->kName, system_ptr));
        } else {
            std::string errmsg = fmt::format("System {} has been registered.",
                                             system_ptr->kName);
            std::cout << errmsg << std::endl;
        }
    }

    // At end of loop
    void ManageEntity() {
        for (const auto &entity : entity_list) {
            if (entity->is_dirty) {
                RegisterSingleEntityToSystem(entity);
            }
        }
        while (!need_to_destroy.empty()) {
            auto entity_ptr = need_to_destroy.front();
            auto iter = entity_list.find(entity_ptr);
            if (iter == entity_list.end()) {
                std::cout << "Shit!" << std::endl;
            }
            std::cout << entity_list.size() << std::endl;
            entity_list.erase(entity_ptr);
            std::cout << entity_list.size() << std::endl;
            need_to_destroy.pop();
        }
    }

    void Destroy(const std::shared_ptr<Entity> &entity) {
        need_to_destroy.push(entity);
    }

    void Destroy(Entity &entity, const std::shared_ptr<Component> &component) {
        entity.RemoveComponent(component->name);
    }

    void LogicalTick(float deltaTime) {
        for (auto &system : system_list) {
            system.second->OnTick(deltaTime);
        }
        ManageEntity();
    }
};
// std::map<std::string, std::>
}  // namespace lunatic_engine