#pragma once
#include <algorithm>
#include <map>
#include <string>
#include <utility>

#include "ImguiComponent.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

using std::map;
using std::string;
namespace lunatic_engine {
class ImguiManager {
   public:
    /**
     * @brief Register a ImguiComponent to the Manager.
     *
     * @param component The component need to register.
     * @return string The component's `uuid` in the manager's map.
     */
    string registerComponent(ImguiComponent& component) {
        string uuid = "";
        // Using the move function to make sure the Component could be handled
        // by the componentMap.
        mComponentMap.insert(std::make_pair(uuid, std::move(component)));
        return uuid;
    }
    void tick() {
        /* std::for_each(
            mComponentMap.begin(), mComponentMap.end(),
            [](ImguiComponent& tempComponent) { tempComponent.onTick(); }); */
    }

   private:
    map<string, ImguiComponent> mComponentMap;
};

}  // namespace lunatic_engine
