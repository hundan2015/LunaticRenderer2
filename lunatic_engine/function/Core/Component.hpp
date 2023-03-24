#pragma once

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
#include "fmt/core.h"
using json = nlohmann::json;
class Component {
    virtual void InitComponent(){};

   public:
    // std::shared_ptr<Entity> entity = nullptr;
    bool enabled = true;
    std::string name;
};
