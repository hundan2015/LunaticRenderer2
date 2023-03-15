#pragma once

#include <algorithm>
#include <cmath>
#include "fmt/core.h"
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
    std::shared_ptr<T> getComponentFromString(std::string component_str) {
        json json_object =
            nlohmann::json_abi_v3_11_2::json::parse(component_str);
        std::shared_ptr<T> result = std::make_shared<T>(json_object.get<T>());
        return result;
    }
};
