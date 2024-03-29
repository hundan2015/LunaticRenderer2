#pragma once

#include "Content.h"
namespace lunatic_engine {
class ImageContent : public Content {
   public:
    std::string image_dir;
    explicit ImageContent(unsigned int _image) : image(_image) {}
    unsigned int image;
};
}  // namespace lunatic_engine
