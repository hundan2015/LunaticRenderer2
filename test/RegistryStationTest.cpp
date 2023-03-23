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
    lunatic_engine::ComponentMeta transform_meta;
    transform_meta.name = "Transform";
    lunatic_engine::Transform transform;
    transform.position_x = 1;
    json component_json = transform;
    transform_meta.component_data = component_json;
    lunatic_engine::RegistryStation *reg =
        lunatic_engine::RegistryStation::GetRegistryStation();
    std::shared_ptr<lunatic_engine::Transform> transform_component =
        std::static_pointer_cast<lunatic_engine::Transform>(
            reg->GetComponent(transform_meta));
    cout << transform_component->position_x;

    lunatic_engine::EntityMeta entity_meta;
    entity_meta.name = "shit";
    entity_meta.components.emplace_back(transform_meta);
    json entity_meta_json = entity_meta;
    std::shared_ptr<lunatic_engine::Entity> entity =
        reg->GetEntity(entity_meta);
    auto transform_ptr_from_entity =
        entity->GetComponent<lunatic_engine::Transform>();
    cout << entity_meta_json << endl;
    cout << "Entity test. Transform x:" << transform_ptr_from_entity->position_x
         << endl;
}