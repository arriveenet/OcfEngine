#pragma once
#include <string>
#include <unordered_map>
#include "base/GameObject.h"
#include "Texture2D.h"

namespace ocf {

class Image;

class TextureManager : public GameObject {
public:
    TextureManager();
    ~TextureManager();

    Texture2D* addImage(const std::string& filePath);
    Texture2D* addImage(Image* pImage, const std::string& key);

    Texture2D* getTextureForKye(const std::string& textureKeyName) const;

    Texture2D* getWhiteTexture();

    Texture2D* getWhiteTexture(const std::string& key, uint8_t luma);

private:
    std::unordered_map<std::string, Texture2D*> m_textures;
};

} // namespace ocf
