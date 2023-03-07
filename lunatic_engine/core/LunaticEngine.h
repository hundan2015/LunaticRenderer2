#pragma once

#include <functional>
#include <memory>
#include <thread>
// The glad must be front of the GLFW.
#include "../function/EntityManager.hpp"
#include "RenderingCore.h"

namespace lunatic_engine {
class LunaticEngine {
    void renderLoop() const;
    std::shared_ptr<RenderingCore> rendering_core_;
    std::shared_ptr<EntityManager> entity_manager_;

   public:
    LunaticEngine();
    void StartEngine() const;
};
}  // namespace lunatic_engine
