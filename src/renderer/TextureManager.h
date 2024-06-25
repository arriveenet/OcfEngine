#pragma once
#include <string>
#include <unordered_map>
#include "base/GameObject.h"
#include "Texture2D.h"

NS_OCF_BEGIN

class TextureManager : public GameObject {
public:
	TextureManager();
	~TextureManager();

	Texture2D* addImage(const std::string& filePath);

private:
	std::unordered_map<std::string, Texture2D*> m_textures;
};

NS_OCF_END
