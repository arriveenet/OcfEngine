#include "SpriteFrameManager.h"
#include "2d/XmlSpriteSheetLoader.h"

using namespace std;

NS_OCF_BEGIN

static SpriteFrameManager* s_sharedSpriteFrameManager = nullptr;

SpriteFrameManager* SpriteFrameManager::getInstance()
{
	if (s_sharedSpriteFrameManager == nullptr) {
		s_sharedSpriteFrameManager = new SpriteFrameManager();
		s_sharedSpriteFrameManager->init();
	}

	return s_sharedSpriteFrameManager;
}

void SpriteFrameManager::destroyInstance()
{
	OCF_SAFE_DELETE(s_sharedSpriteFrameManager);
}

SpriteFrameManager::~SpriteFrameManager()
{
}

bool SpriteFrameManager::init()
{
	m_spriteFrames.reserve(20);
	clear();

	registerSpriteSheetLoader(std::make_shared<XmlSpriteSheetLoader>());

	return true;
}

void SpriteFrameManager::addSpriteFramesWithFile(std::string_view spriteSheetFilename, uint32_t spriteSheetFormat)
{
	auto* loader = getSpriteSheetLoader(spriteSheetFormat);
	if (loader != nullptr) {
		loader->load(spriteSheetFilename, *this);
	}
}

SpriteFrame* SpriteFrameManager::findFrame(std::string_view frame)
{
	return m_spriteFrames.at(std::string(frame)).get();
}

SpriteFrame* SpriteFrameManager::getSpriteFrameByName(std::string_view name)
{
	auto* frame = findFrame(name);
	if (frame == nullptr) {
		
	}
	return frame;
}

void SpriteFrameManager::insertFrame(const std::shared_ptr<SpriteSheet>& spriteSheet, std::string_view frameName, SpriteFrame* spriteFrame)
{
	//spriteSheet->frames.insert(std::string(frameName));
	std::pair<std::string, std::shared_ptr<SpriteFrame>> frameItem(frameName, spriteFrame);
	m_spriteFrames.insert(frameItem);
}

void SpriteFrameManager::clear()
{
	m_spriteFrames.clear();
	m_spriteSheets.clear();
}

void SpriteFrameManager::registerSpriteSheetLoader(std::shared_ptr<ISpriteSheetLoader> loader)
{
	const auto format = loader->getFormat();
	if (m_spriteSheetLoader.find(format) != m_spriteSheetLoader.end()) {
		return;
	}

	m_spriteSheetLoader.emplace(format, std::move(loader));
}

ISpriteSheetLoader* SpriteFrameManager::getSpriteSheetLoader(uint32_t spriteSheetFormat)
{
	auto&& iter = m_spriteSheetLoader.find(spriteSheetFormat);
	if (iter != m_spriteSheetLoader.end()) {
		return iter->second.get();
	}

	return nullptr;
}

SpriteFrameManager::SpriteFrameManager()
{

}

NS_OCF_END
