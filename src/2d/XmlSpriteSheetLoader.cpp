#include "XmlSpriteSheetLoader.h"
#include "2d/SpriteFrame.h"
#include "2d/SpriteFrameManager.h"
#include "base/Game.h"
#include "base/FileUtils.h"
#include <tinyxml2.h>

NS_OCF_BEGIN

void XmlSpriteSheetLoader::load(std::string_view filePath, SpriteFrameManager& manager)
{
	const auto fullPath = FileUtils::getInstance()->fullPathForFilename(filePath.data());
	if (fullPath.empty()) {
		return;
	}

	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(fullPath.c_str()) != tinyxml2::XML_SUCCESS) {
		return;
	}

	auto spriteSheet = std::make_shared<SpriteSheet>();
	spriteSheet->format = getFormat();
	spriteSheet->path = fullPath;

	tinyxml2::XMLElement* textureAtlas = doc.FirstChildElement("TextureAtlas");
	if (textureAtlas == nullptr) {
		return;
	}
	auto imagePath = textureAtlas->Attribute("imagePath");
	auto width = textureAtlas->IntAttribute("width");
	auto height = textureAtlas->IntAttribute("height");

	auto imageFullPath = FileUtils::getInstance()->fullPathForFilename(imagePath);
	const auto texture = Game::getInstance()->getTextureManager()->addImage(imageFullPath);

	for (tinyxml2::XMLElement* spriteElement = textureAtlas->FirstChildElement("sprite");
		spriteElement != nullptr;
		spriteElement = spriteElement->NextSiblingElement()) {
		auto name = spriteElement->Attribute("n");
		auto x = spriteElement->FloatAttribute("x");
		auto y = spriteElement->FloatAttribute("y");
		auto w = spriteElement->FloatAttribute("w");
		auto h = spriteElement->FloatAttribute("h");

		auto spriteFrame = SpriteFrame::createWithTexture(texture, Rect(x, y, w, h), false, glm::vec2(), glm::vec2(width, height));

		manager.insertFrame(spriteSheet, name, spriteFrame);
	}
}

NS_OCF_END
