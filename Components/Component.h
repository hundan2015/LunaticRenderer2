#pragma once
namespace LunaticEngine {
class Component {
   public:
    bool mEnabled = true;
    virtual void onTick();
    virtual void onStart();
};
}  // namespace LunaticEngine