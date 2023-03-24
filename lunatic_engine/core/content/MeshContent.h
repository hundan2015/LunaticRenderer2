#pragma once
#include "Content.h"
namespace lunatic_engine {
class MeshContent : public Content {
   public:
    unsigned int vao;
    unsigned int triangle_count;
};
class MeshContentNode {
   public:
    int num;
    std::shared_ptr<MeshContent> mesh_content;
    std::vector<std::shared_ptr<MeshContentNode>> children;
};
// TODO: Here need some design! The mesh info is too foolish!
class MeshInfo {
   public:
    std::shared_ptr<MeshContentNode> root;
    std::vector<std::shared_ptr<MeshContent>> mesh_list;
};
}  // namespace lunatic_engine
