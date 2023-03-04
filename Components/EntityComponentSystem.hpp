#pragma once
#include <vcruntime_typeinfo.h>
#include <algorithm>
#include <cmath>
#include <format>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <set>
#include <map>
#include <typeinfo>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <unordered_map>
#include <utility>
#include <vector>

using json = nlohmann::json;
namespace LunaticEngine {
template <typename T>
class RegisterComponent {
   public:
    RegisterComponent() {
        std::cout << typeid(T).name() << " has registered." << std::endl;
    }
};
class Entity;
class Component {
    virtual void initComponent(){};

   public:
    // std::shared_ptr<Entity> entity = nullptr;
    bool mEnabled = true;
    template <typename T>
    std::string toString() {
        json j = *static_cast<T*>(this);
        return j;
    }
    template <typename T>
    std::shared_ptr<T> getComponentFromString(std::string componentStr) {
        json j = json::parse(componentStr);
        std::shared_ptr<T> result = std::make_shared<T>(j.get<T>());
        return result;
    }
};

class Entity {
   private:
    std::string mName;

   public:
    Entity() = default;
    std::string getName() { return mName; }
    bool isDirty = false;
    std::map<std::string, std::shared_ptr<Component>> mComponents;
    template <typename T>
    const std::shared_ptr<T> getComponent() {
        std::string componentID = typeid(T).name();
        auto result = mComponents.find(componentID);
        if (result != mComponents.end()) {
            return std::static_pointer_cast<T>(result->second);
        } else {
            std::string errMessage = std::format(
                "WARNING::Entity {} have no {}", mName, componentID);
            // TODO:Log warning.
            return nullptr;
        }
    }
    template <typename T>
    void addComponent(std::shared_ptr<T> component) {
        std::shared_ptr<Component> componentTemp =
            std::static_pointer_cast<Component>(component);
        mComponents.insert(std::make_pair(typeid(T).name(), componentTemp));
        // componentTemp->entity = std::make_shared<Entity>(this);
        isDirty = true;
    }
    void removeComponent(std::string component) {
        isDirty = true;
        mComponents.erase(component);
    }
};
class System {
   protected:
    std::set<std::shared_ptr<Entity>> mTargets;
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
    const std::string kName;
    void registerToSystem(std::shared_ptr<Entity> entity) {
        mTargets.insert(entity);
    }
    std::set<std::string> mRequiredComponents;
    System(std::string name) : kName(name) {
        // std::sort(mRequiredComponents.begin(), mRequiredComponents.end());
        std::string componentNameSeq = "";
        for (auto& componentName : mRequiredComponents) {
            componentNameSeq += ":" + componentName;
        }
        std::hash<std::string> hashCode;
        mHashCode = static_cast<int>(hashCode(componentNameSeq));
        // TODO: Take it to the static register tree.
    }
    int mHashCode;
    virtual void onStart(){};
    virtual void onTick(float deltaTime){};
    virtual void onDisabled(){};
};

}  // namespace LunaticEngine