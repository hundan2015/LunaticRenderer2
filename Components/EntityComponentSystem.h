#pragma once
#include <vcruntime_typeinfo.h>
#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <set>
#include <map>
#include <typeinfo>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <unordered_map>
#include <vector>

#define COMPONENT(x, y)                             \
    class x {                                       \
        friend void to_json(json& j, const x& p);   \
        friend void from_json(const json& j, x& p); \
        y                                           \
    }

using json = nlohmann::json;
namespace LunaticEngine {

class Component {
    virtual void initComponent();

   public:
    bool mEnabled = true;
    template<typename T>
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
    std::map<std::string, std::shared_ptr<Component>> mComponents;
    std::string mName;

   public:
    template <typename T>
    const std::shared_ptr<T> getComponent() {
        std::string componentID = typeid(T).name();
        auto result = mComponents.find(componentID);
        if (result != mComponents.end()) {
            return result->second;
        } else {
            std::string errMessage =
                std::format("ERROR::Entity {} have no {}", mName, componentID);
            // TODO:Log error.
            return nullptr;
        }
    }
};
class System {
   protected:
    std::unordered_map<std::string, Entity> mTargets;
    std::vector<std::string> mRequiredComponents;
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
    System() {
        std::sort(mRequiredComponents.begin(), mRequiredComponents.end());
        std::string componentNameSeq = "";
        for (auto& componentName : mRequiredComponents) {
            componentNameSeq += ":" + componentName;
        }
        std::hash<std::string> hashCode;
        mHashCode = static_cast<int>(hashCode(componentNameSeq));
        // TODO: Take it to the static register tree.
    }
    int mHashCode;
    virtual void onStart();
    virtual void onTick(float deltaTime);
    virtual void onDisabled();
};

}  // namespace LunaticEngine