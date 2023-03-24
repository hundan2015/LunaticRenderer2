#pragma once
#include "Component.hpp"
#include "System.hpp"
#include "map"
using json = nlohmann::json;
namespace lunatic_engine {
struct ComponentMeta {
    std::string name;
    json component_data;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ComponentMeta, name, component_data);

struct EntityMeta {
    std::string name;
    std::vector<ComponentMeta> components;
    std::vector<EntityMeta> child;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(EntityMeta, name, components, child);

class RegistryStation {
    std::map<std::string, std::function<std::shared_ptr<Component>(json)>>
        registry_map_;
    std::map<std::string, std::function<json(std::shared_ptr<Component>)>>
        come_back_map_;

   public:
    void RegisterToMap(std::string name,
                       std::function<std::shared_ptr<Component>(json)> func) {
        auto pre_search = registry_map_.find(name);
        if (pre_search != registry_map_.end()) {
            std::cout << "WARNING::" << name
                      << " has been registerd to registry map." << std::endl;
            return;
        }
        registry_map_.insert(std::make_pair(name, func));
    }
    void RegisterToComeBackMap(
        std::string name,
        std::function<json(std::shared_ptr<Component>)> func) {
        auto pre_search = come_back_map_.find(name);
        if (pre_search != come_back_map_.end()) {
            std::cout << "WARNING::" << name
                      << " has been registerd to come back map." << std::endl;
            return;
        }
        come_back_map_.insert(std::make_pair(name, func));
    }
    // RegistryStation(RegistryStation&) = delete;
    static RegistryStation* GetRegistryStation() {
        if (!registry_station_) {
            std::cout << "Have no Station.\n";
            registry_station_ = new RegistryStation();
        }
        return registry_station_;
    }
    static RegistryStation* registry_station_;

    std::shared_ptr<Component> GetComponent(
        const ComponentMeta& component_wrapper) {
        std::string component_name = component_wrapper.name;
        std::cout << "RegisterStation::Name is " << component_name << std::endl;
        std::cout << "RegisterStation::Station has " << registry_map_.size()
                  << std::endl;
        auto func = registry_map_.find(component_name);
        if (func == registry_map_.end()) {
            std::cout << "ERROR::No this kind of component." << std::endl;
            abort();
            // return nullptr;
        }
        std::shared_ptr<Component> target_component =
            (func->second)(component_wrapper.component_data);
        return target_component;
    }
    std::shared_ptr<Entity> GetEntity(EntityMeta& entity_meta) {
        // auto shit = ;
        // Must use the Entity(name) constructor, or get the error.
        std::shared_ptr<Entity> entity_root(new Entity("temp"));
        if (entity_meta.name != "") {
            entity_root->name = entity_meta.name;
        }
        for (auto& component_meta : entity_meta.components) {
            auto component_ptr = GetComponent(component_meta);
            // auto shit = decltype(*component_ptr);
            std::cout << typeid(*component_ptr).name() << std::endl;
            entity_root->AddComponent(component_ptr);
        }
        for (auto& child_entity : entity_meta.child) {
            auto child = GetEntity(child_entity);
            child->parent = entity_root;
            entity_root->child.emplace_back(child);
        }
        return entity_root;
    }
    EntityMeta GetEntityMeta(std::shared_ptr<Entity> entity) {
        EntityMeta res;
        res.name = entity->name;
        for (auto& component : entity->components) {
            ComponentMeta component_meta = GetComponentMeta(component.second);
            res.components.emplace_back(component_meta);
        }
        for (auto child : entity->child) {
            res.child.emplace_back(GetEntityMeta(child));
        }
        return res;
    }
    ComponentMeta GetComponentMeta(
        const std::shared_ptr<Component>& component) {
        ComponentMeta component_meta;
        component_meta.name = component->name;
        auto json_function = come_back_map_.find(component_meta.name);
        if (json_function != come_back_map_.end()) {
            component_meta.component_data = json_function->second(component);
        } else {
            std::cout << "ERROR::The component " << component_meta.name
                      << "has not registered!" << std::endl;
            abort();
        }
        return component_meta;
    }
};
/**
 * Component, System need to be registered.
 * @tparam T
 */
template <typename T>
class RegisterHelper {
   public:
    RegisterHelper(std::string name) {
        RegistryStation* registry_station =
            RegistryStation::GetRegistryStation();
        auto generate_function = [](json component_json) {
            T component = component_json.get<T>();
            std::shared_ptr<T> component_ptr = std::make_shared<T>(component);
            return (component_ptr);
        };
        auto come_back_function = [](std::shared_ptr<Component> component) {
            auto ptr = std::static_pointer_cast<T>(component);
            json result_json = *ptr;
            std::cout << "Come back result = " << result_json << std::endl;
            return result_json;
        };
        registry_station->RegisterToMap(name, generate_function);
        registry_station->RegisterToComeBackMap(name, come_back_function);
        std::cout << name << " has been called to register." << std::endl;
    }
};
}  // namespace lunatic_engine
