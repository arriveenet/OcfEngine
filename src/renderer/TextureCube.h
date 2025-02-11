#pragma once
#include "base/GameObject.h"
#include "base/Types.h"
#include <string>
#include <vector>

NS_OCF_BEGIN

class Image;

class TextureCube : public GameObject {
public:
    static TextureCube* create(std::string_view positive_x,
                               std::string_view negative_x,
                               std::string_view positive_y,
                               std::string_view negative_y,
                               std::string_view positive_z,
                               std::string_view negative_z);

    TextureCube();
    virtual ~TextureCube();

    bool init(std::string_view positive_x,
              std::string_view negative_x,
              std::string_view positive_y,
              std::string_view negative_y,
              std::string_view positive_z,
              std::string_view negative_z);

protected:
    Image* createImage(std::string_view filename);

protected:
    std::vector<std::string> m_imageFiles;
    unsigned int m_textureId;
};

NS_OCF_END
