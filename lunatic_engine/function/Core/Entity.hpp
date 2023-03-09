#pragma once

#include <vcruntime_typeinfo.h>
#include <algorithm>
#include <cmath>
#include <format>
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
    std::string name_;

   public:
    std::weak_ptr<Entity> parent;
    std::vector<std::shared_ptr<Entity>> child;

    Entity() = default;
    std::string GetName() { return name_; }
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
            std::format("WARNING::Entity {} have no {}", name_, component_id);
        // TODO(Symbolic): Log warning.
        return nullptr;
    }
    bool TestIsEnabled(const std::set<std::string>& requried_components) {
        for (const auto& component_id : requried_components) {
            auto result = components.find(component_id);
            if (result != components.end() && !result->second->enabled) {
                return false;
            }
            if (result == components.end()) return false;
        }
        return true;
    }
    template <typename T>
    void addComponent(std::shared_ptr<T> component) {
        std::shared_ptr<Component> component_temp =
            std::static_pointer_cast<Component>(component);
        components.insert(std::make_pair(typeid(T).name(), component_temp));
        // component_temp->entity = std::make_shared<Entity>(this);
        is_dirty = true;
    }
    void RemoveComponent(const std::string& component) {
        is_dirty = true;
        components.erase(component);
    }
};
}  // namespace lunatic_engine
