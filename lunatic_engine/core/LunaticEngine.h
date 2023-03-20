#pragma once

#include <functional>
#include <memory>
#include <thread>
// The glad must be front of the GLFW.
#include "../function/Core/EntityManager.hpp"
#include "RenderingCore.h"
#include "ResourceCore.h"

namespace lunatic_engine {
class LunaticEngine {
    void renderLoop() const;
    std::shared_ptr<RenderingCore> rendering_core_;
    /**
     * TODO: Entity Manager need a decoration. It should be a context, which not
     * related to the engine itself.
     */

    std::shared_ptr<EntityManager> entity_manager_;
    std::shared_ptr<ResourceCore> resource_core_;

   public:
    LunaticEngine();
    void StartEngine() const;
};
}  // namespace lunatic_engine
