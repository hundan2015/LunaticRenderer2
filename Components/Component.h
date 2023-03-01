#pragma once
#include <format>
#include <memory>
#include <string>
#include <set>
#include <map>
#include <typeinfo>
namespace LunaticEngine {
class ComponentOld {
   public:
    bool mEnabled = true;
    virtual void onTick();
    virtual void onStart();
    virtual std::string toString();
    virtual void stringSerial(std::string componentStr);
};

}  // namespace LunaticEngine