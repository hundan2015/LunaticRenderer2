#pragma once

#include <fmt/core.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <ostream>
#include <set>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>
#include "Component.hpp"
namespace lunatic_engine {
class Entity {
   private:
   public:
    std::string name;
    std::weak_ptr<Entity> parent;
    std::vector<std::shared_ptr<Entity>> child;

    Entity(std::string name_) : name(name_){};
    Entity() = default;
    std::string GetName() { return name; }
    bool is_dirty = false;
    std::map<std::string, std::shared_ptr<Component>> components;
    template <typename T>
    std::shared_ptr<T> GetComponent() {
        std::string component_id = typeid(T).name();
        auto result = components.find(component_id);
        if (result != components.end()) {
            return std::static_pointer_cast<T>(result->second);
        }
        std::string err_message =
            fmt::format("WARNING::Entity {} have no {}", name, component_id);
        std::cout << err_message << std::endl;
        return nullptr;
    }
    /**
     *
     * @param requried_components
     * @return Is the entity need to caculate.
     * @warning This method should no longer available.
     * Because it runs too slow, maybe we should use remove or add component
     * to do these things. Or we should not use it directly.
     */
    bool TestIsEnabled(const std::set<std::string> &requried_components) {
        for (const auto &component_id : requried_components) {
            auto result = components.find(component_id);
            if (result != components.end() && !result->second->enabled) {
                return false;
            }
            if (result == components.end()) return false;
        }
        return true;
    }
    template <typename T>
    void AddComponent(std::shared_ptr<T> component) {
        std::shared_ptr<Component> component_temp =
            std::static_pointer_cast<Component>(component);
        components.insert(std::make_pair(typeid(*component).name(), component_temp));
        // component_temp->entity = std::make_shared<Entity>(this);
        is_dirty = true;
    }
    void RemoveComponent(const std::string &component) {
        is_dirty = true;
        components.erase(component);
    }
};
}  // namespace lunatic_engine
