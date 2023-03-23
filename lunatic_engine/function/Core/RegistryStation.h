#pragma once
#include "Component.hpp"
#include "Entity.hpp"
#include "System.hpp"
#include "map"
using json = nlohmann::json;
namespace lunatic_engine {
struct ComponentMeta {
    std::string name;
    json component_data;
};
class RegistryStation {
    std::map<std::string, std::function<std::shared_ptr<Component>(json)>>
        registry_map_;

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
};

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
        registry_station->RegisterToMap(name, generate_function);
        std::cout << name << " has been called to register." << std::endl;
    }
};
}  // namespace lunatic_engine
