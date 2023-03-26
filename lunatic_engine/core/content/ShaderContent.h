#pragma once
#include "Content.h"
namespace lunatic_engine {
class ShaderContent : public Content {
   public:
    std::string vertex_shader_dir;
    std::string fragment_shader_dir;
    unsigned int shader_program;
};
}  // namespace lunatic_engine
