#pragma once
#include <string>
namespace LunaticEngine {
class Component {
   public:
    bool mEnabled = true;
    virtual void onTick();
    virtual void onStart();
    virtual std::string toString();
    virtual void stringSerial(std::string componentStr);
};
}  // namespace LunaticEngine