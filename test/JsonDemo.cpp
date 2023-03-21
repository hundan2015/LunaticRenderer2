//
// Created by Symbolic on 2023/3/21.
//
#include "iostream"
#include "map"
#include "nlohmann/json.hpp"
using json = nlohmann::json;
using std::cout;
struct TrueComponent {
    int x = 0;
    int y = 0;
};
struct Component {};
struct BStruct : Component {
    int x = 0;
    int y = 0;
    int z = 0;
    TrueComponent a;
    std::vector<int> vec;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TrueComponent, x, y);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BStruct, x, y, z, a, vec);
std::map<std::string, std::function<std::unique_ptr<Component>(json)>> shit;
template <typename T>
class RegisterShit {
   public:
    RegisterShit(std::string name) {
        shit.insert(std::make_pair(
            name, [](json j) { return std::make_unique<T>(j.get<T>()); }));
        cout << "Register Complete.\n";
    }
};
// Register Component BStruct.
static RegisterShit<BStruct> register_shit = RegisterShit<BStruct>("BStruct");

int main() {
    auto func = shit.find("BStruct");
    BStruct b_struct;
    b_struct.x = 1;
    b_struct.a.x = 2;
    b_struct.vec.push_back(1);
    json j1 = b_struct;
    auto ptr2 = func->second(j1);
    // Special way to cast ptr2 into BStruct.
    auto *ptr3 = (BStruct *)(ptr2.get());
    ptr2.release();
    std::unique_ptr<BStruct> ptr = std::make_unique<BStruct>();
    ptr.reset(ptr3);
    // Release doesn't destroy the memory, the reset does.
    cout << ptr->x << " " << ptr->a.x << " " << ptr->vec.front();
}