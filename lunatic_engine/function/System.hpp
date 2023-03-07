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
#include "Entity.hpp"

namespace lunatic_engine {
template <typename T>
class RegisterComponent {
   public:
    RegisterComponent() {
        std::cout << typeid(T).name() << " has registered." << std::endl;
    }
};

class System {
   protected:
    int hash_code;
    std::set<std::shared_ptr<Entity>> targets_;
    template <class T>
    void registerSystemToManager() {
        // Register to SystemManager.
        std::string system_name = typeid(T).name();
        int register_result = 0;
        switch (register_result) {
            case 0:
                std::cout << std::format(
                                 "[Debug] System {} has registered to the "
                                 "system manager.",
                                 system_name)
                          << std::endl;
                break;
            case 1:
                std::cout << std::format(
                    "[Warning] System {} has been registered.", system_name);
                break;
        }
    }

   public:
    int mPriority;
    const std::string kName;
    void RegisterToSystem(const std::shared_ptr<Entity>& entity) {
        targets_.insert(entity);
    }
    std::set<std::string> required_components;
    bool IsEntityEnabled(std::shared_ptr<Entity> entity) {
        return entity->TestIsEnabled(required_components);
    }
    explicit System(std::string name) : kName(std::move(name)) {
        // std::sort(required_components.begin(), required_components.end());
        std::string component_name_seq;
        for (const auto& component_name : required_components) {
            component_name_seq += ":" + component_name;
        }
        std::hash<std::string> hash_code_string;
        hash_code = static_cast<int>(hash_code_string(component_name_seq));
        // TODO(Symbolic): Take it to the static register tree.
    }

    virtual void OnStart(){};
    virtual void OnTick(float deltaTime) {
        std::cout << "Do nothing" << deltaTime << std::endl;
    };
    virtual void OnDisabled(){};
};

}  // namespace lunatic_engine