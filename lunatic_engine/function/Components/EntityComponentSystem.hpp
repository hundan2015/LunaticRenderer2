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

using json = nlohmann::json;
namespace lunatic_engine {
class Entity;
template <typename T>
class RegisterComponent {
   public:
    RegisterComponent() {
        std::cout << typeid(T).name() << " has registered." << std::endl;
    }
};
class Component {
    virtual void InitComponent(){};

   public:
    // std::shared_ptr<Entity> entity = nullptr;
    bool enabled = true;
    std::string name;
    template <typename T>
    std::string toString() {
        json json_object = *static_cast<T*>(this);
        return json_object;
    }
    template <typename T>
    std::shared_ptr<T> getComponentFromString(std::string componentStr) {
        json json_object = json::parse(componentStr);
        std::shared_ptr<T> result = std::make_shared<T>(json_object.get<T>());
        return result;
    }
};

class Entity {
   private:
    std::string name_;

   public:
    std::weak_ptr<Entity> parent;
    std::vector<std::shared_ptr<Entity>> child;

    Entity() = default;
    std::string GetName() { return name_; }
    bool is_dirty = false;
    std::map<std::string, std::shared_ptr<Component>> mComponents;
    template <typename T>
    std::shared_ptr<T> getComponent() {
        std::string component_id = typeid(T).name();
        auto result = mComponents.find(component_id);
        if (result != mComponents.end()) {
            return std::static_pointer_cast<T>(result->second);
        }
        std::string errMessage =
            std::format("WARNING::Entity {} have no {}", name_, component_id);
        // TODO(Symbolic): Log warning.
        return nullptr;
    }
    template <typename T>
    void addComponent(std::shared_ptr<T> component) {
        std::shared_ptr<Component> componentTemp =
            std::static_pointer_cast<Component>(component);
        mComponents.insert(std::make_pair(typeid(T).name(), componentTemp));
        // componentTemp->entity = std::make_shared<Entity>(this);
        is_dirty = true;
    }
    void RemoveComponent(const std::string& component) {
        is_dirty = true;
        mComponents.erase(component);
    }
};
class System {
   protected:
    std::set<std::shared_ptr<Entity>> mTargets_;
    void registerSystemToManager() {
        // Register to SystemManager.
        std::string systemName = typeid(*this).name();
        int registerResult = 0;
        switch (registerResult) {
            case 0:
                std::cout << std::format(
                                 "[Debug] System {} has registered to the "
                                 "system manager.",
                                 systemName)
                          << std::endl;
                break;
            case 1:
                std::cout << std::format(
                    "[Warning] System {} has been registered.", systemName);
                break;
        }
    }

   public:
    int mPriority;
    const std::string kName;
    void registerToSystem(const std::shared_ptr<Entity>& entity) {
        mTargets_.insert(entity);
    }
    std::set<std::string> required_components;
    explicit System(std::string name) : kName(std::move(name)) {
        // std::sort(required_components.begin(), required_components.end());
        std::string componentNameSeq;
        for (const auto& componentName : required_components) {
            componentNameSeq += ":" + componentName;
        }
        std::hash<std::string> hashCode;
        mHashCode = static_cast<int>(hashCode(componentNameSeq));
        // TODO(Symbolic): Take it to the static register tree.
    }
    int mHashCode;
    virtual void OnStart(){};
    virtual void OnTick(float deltaTime) {
        std::cout << "Do nothing" << deltaTime << std::endl;
    };
    virtual void OnDisabled(){};
};

}  // namespace lunatic_engine