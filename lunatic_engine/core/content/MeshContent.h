#pragma once
#include "Content.h"
namespace lunatic_engine {
class MeshContent: public Content{
   public:
    unsigned int vao;
    unsigned int triangle_count;
};
}  // namespace lunatic_engine
