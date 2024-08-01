#include "TextureManager.h"
#include "Image.h"
#include "base/FileUtils.h"

NS_OCF_BEGIN

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
			OCF_SAFE_RELEASE(texture);
			texture = nullptr;
			OCFLOG("ocf: initWithImage failed!");
		}
		else {
			m_textures.emplace(fullPath, texture);
		}
	}

	return texture;
}

Texture2D* TextureManager::addImage(Image* pImage, const std::string& key)
{
	Texture2D* texture = nullptr;

	auto iter = m_textures.find(key);
	if (iter != m_textures.end()) {
		texture = iter->second;
	}

	if (!texture) {
		texture = new Texture2D();
		if (!texture->initWithImage(pImage, pImage->getPixelFormat())) {
			OCF_SAFE_RELEASE(texture);
			texture = nullptr;
			OCFLOG("ocf: initWithImage failed!");
		}
		else {
			m_textures.emplace(key, texture);
		}
	}

	return texture;
}

Texture2D* TextureManager::getTextureForKye(const std::string& textureKeyName) const
{
	auto iter = m_textures.find(textureKeyName);

	if (iter != m_textures.end()) {
		auto key = FileUtils::getInstance()->fullPathForFilename(textureKeyName);
		iter = m_textures.find(key);
	}

	if (iter != m_textures.end()) {
		return iter->second;
	}

	return nullptr;
}

Texture2D* TextureManager::getWhiteTexture()
{
	const std::string key = "/white-texture";
	return getWhiteTexture(key, 0xff);
}

Texture2D* TextureManager::getWhiteTexture(const std::string& key, uint8_t luma)
{
	Texture2D* texture = getTextureForKye(key);
	if (texture != nullptr) {
		return texture;
	}

	const uint8_t texls[] = {
		luma, luma, luma, 0xff, luma, luma,luma, 0xff,
		luma, luma, luma, 0xff, luma, luma,luma, 0xff
	};

	Image image;
	bool isOK = image.initWithRowData(texls, sizeof(texls), 2, 2);
	OCFASSERT(isOK, "The 2x2 empty texture was created unsuccessfully.");

	texture = addImage(&image, key);
	return texture;
}

NS_OCF_END
