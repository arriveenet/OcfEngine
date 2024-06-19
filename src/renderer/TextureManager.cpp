#include "TextureManager.h"
#include "Image.h"
#include "base/FileUtils.h"

OCF_BEGIN

TextureManager::TextureManager()
{
}

TextureManager::~TextureManager()
{
	for (auto& texture : m_textures) {
		texture.second->release();
	}
}

Texture2D* TextureManager::addImage(const std::string& filePath)
{
	Texture2D* texture = nullptr;

	std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filePath);
	if (fullPath.empty()) {
		return nullptr;
	}

	auto iter = m_textures.find(fullPath);
	if (iter != m_textures.end()) {
		texture = iter->second;
	}

	if (!texture) {
		texture = new Texture2D();
		if (!texture->initWithFile(fullPath)) {
			delete texture;
			texture = nullptr;
		}
		else {
			m_textures.emplace(fullPath, texture);
		}
	}

	return texture;
}

OCF_END
