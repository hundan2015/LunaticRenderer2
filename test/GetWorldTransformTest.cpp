#include <iostream>
#include "../core/ResourceCore.h"
#include "Systems/RenderSystem.hpp"
#include "fmt/core.h"
using namespace lunatic_engine;
using std::cout;
using std::endl;
int main() {
    RenderingSystem rendering_system;
    std::shared_ptr<Entity> entity1 = std::make_shared<Entity>();
    std::shared_ptr<Entity> entity2 = std::make_shared<Entity>();
    entity2->parent = entity1;

    std::shared_ptr<Transform> transform1 = std::make_shared<Transform>();
    std::shared_ptr<Transform> transform2 = std::make_shared<Transform>();

    transform1->position_x = 1;
    transform2->position_x = 1;

    transform1->position_y = -1;
    transform2->position_y = 1;

    transform1->scale_x = 2;
    transform2->scale_x = 1;

    entity1->AddComponent<Transform>(transform1);
    entity2->AddComponent<Transform>(transform2);

    std::shared_ptr<Transform> transform_test = std::make_shared<Transform>();
    rendering_system.GetWorldTransform(transform_test, entity2);
    cout << fmt::format("x:{},y:{},z:{}\nscale_x:{},scale_y:{},scale_z:{}",
                        transform_test->position_x, transform_test->position_y,
                        transform_test->position_z, transform_test->scale_x,
                        transform_test->scale_y, transform_test->scale_z)
         << endl;
}
