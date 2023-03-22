//
// Created by symbolic on 23-3-22.
//
#include <memory>
#include "iostream"
using std::cout;
struct AStruct {
    int x = 1;
    float y;
};
int main() {
    // Use unique ptr to allocate the specific memory.
    std::unique_ptr<char> ptr_original(new char[50]);
    auto *ptr2 = ptr_original.release();
    cout << ptr2 << std::endl;
    // cast ptr to AStruct ptr.
    auto *ptr_temp = reinterpret_cast<AStruct *>(ptr2);
    std::shared_ptr<AStruct> ptr3(ptr_temp);
    cout << ptr3.get() << std::endl;
    // Re-construct the object in the memory.
    *ptr3 = AStruct();
    cout << ptr3->x << " " << ptr3->y << std::endl;
    cout << ptr3.get() << std::endl;
    cout << ptr3.use_count();
}