#pragma once
#include <string>
#include <unordered_map>
#include "GameObject.h"
#include "Texture2D.h"

class TextureManager : public GameObject {
public:
	TextureManager();
	~TextureManager();

	Texture2D* addImage(const std::string& filePath);

private:
	std::unordered_map<std::string, Texture2D*> m_textures;
};