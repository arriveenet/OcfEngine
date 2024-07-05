#pragma once
#include <string>
#include "base/GameObject.h"

NS_OCF_BEGIN

enum class SpriteSheetFormat : uint32_t {
	Json,
	Custom
};

class SpriteSheet {
public:
	std::string path;
	uint32_t format;
};

class ISpriteSheetLoader {
public:
	virtual ~ISpriteSheetLoader() = default;
	virtual uint32_t getFormat()                   = 0;
	virtual void load(const std::string& filePath) = 0;
};

NS_OCF_END