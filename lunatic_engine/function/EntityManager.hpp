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

    void RegisterToSystem(const std::shared_ptr<Entity> &entity) {
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
            std::ranges::set_difference(entity_have.begin(), entity_have.end(),
                                        system_map.begin(), system_map.end(),
                                        std::back_inserter(res));
            if (res.empty()) {
                system.second->RegisterToSystem(entity);
                std::cout << "Entity " << entity->GetName()
                          << " has registered to " << system.second->kName
                          << std::endl;
                have_a_system = true;
            }
        }
        if (!have_a_system) {
            need_to_destroy.push(entity);
        }
    }

    void RegisterSystem(const std::shared_ptr<System> &system_ptr) {
        auto iter = system_list.find(system_ptr->kName);
        if (iter == system_list.end()) {
            system_list.insert(std::make_pair(system_ptr->kName, system_ptr));
        } else {
            std::string errmsg = std::format("System {} has been registered.",
                                             system_ptr->kName);
            std::cout << errmsg << std::endl;
        }
    }

    // At end of loop
    void ManageEntity() {
        for (const auto &entity : entity_list) {
            if (entity->is_dirty) {
                RegisterToSystem(entity);
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