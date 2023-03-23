//
// Created by symbolic on 23-3-23.
//
#include "Components/Transform.h"
#include "Core/RegistryStation.h"
#include "iostream"

using std::cout;
using std::endl;
using json = nlohmann::json;

int main() {
    lunatic_engine::ComponentMeta meta;
    meta.name = "Transform";
    lunatic_engine::Transform transform;
    transform.position_x = 1;
    json component_json = transform;
    meta.component_data = component_json;
    lunatic_engine::RegistryStation *reg =
        lunatic_engine::RegistryStation::GetRegistryStation();
    std::shared_ptr<lunatic_engine::Transform> transform_component =
        std::static_pointer_cast<lunatic_engine::Transform>(
            reg->GetComponent(meta));
    cout << transform_component->position_x;
}