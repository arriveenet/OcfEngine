#include "Font.h"

#include <memory>
#include <stdio.h>
#include <stdarg.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include "Game.h"
#include "base/FileUtils.h"

NS_OCF_BEGIN

using namespace glm;

Font::Font()
	: m_projection(1.0f)
	, m_texture(nullptr)
	, m_fntInfo()
	, m_fntCommon()
{
}

Font::~Font()
{
	m_chars.clear();

	OCF_SAFE_RELEASE(m_textureAtlas)
	OCF_SAFE_RELEASE(m_texture)
}

bool Font::init(const std::string& filename)
{
	auto fullpath = FileUtils::getInstance()->fullPathForFilename(filename);
	if (!parseFntFile(fullpath)) {
		return false;
	}

	m_texture = new Texture2D();
	
	std::string parentPath = FileUtils::getInstance()->getParentFullPath(fullpath);
	if (!m_texture->initWithFile(parentPath + "\\" + m_pageName)) {
		delete m_texture;
		m_texture = nullptr;
		return false;
	}

	vec2 windowSize = Game::getInstance()->getVisibleSize();
	m_projection = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f);

	setupTextureAtlas(".\\resource\\Consolas_0.png");

	return true;
}

const FntChars& Font::getChar(unsigned int id) const
{
	auto iter = m_chars.find(id);
	if (iter != m_chars.end()) {
		return iter->second;
	}

	assert(false);
	static FntChars fntChar;
	return fntChar;
}

bool Font::parseFntFile(const std::string& fntFile)
{
	FILE* pFile = nullptr;
	errno_t err = fopen_s(&pFile, fntFile.c_str(), "rb");

	if (err != 0) {
		return false;
	}

	do {
		// FNTファイルを判定する
		unsigned char fileType[4];
		fread(fileType, sizeof(fileType), 1, pFile);
		if (!isFnt(fileType, sizeof(fileType))) {
			break;
		}

		// FNTファイルの各ブロックを読み込む
		FntBlock block;
		while (readFntBlock(&block, pFile)) {
			switch (block.type) {
			case FntBlockType::Info:
				readFntInfo(block, m_fntInfo, pFile);
				break;
			case FntBlockType::Common:
				readFntCommon(block, m_fntCommon, pFile);
				break;
			case FntBlockType::Pages:
				readFntPages(block, pFile);
				break;
			case FntBlockType::Chars:
				readFntChars(block, pFile);
				break;
			case FntBlockType::KerningPairs:
				FntKerningPairs kerningPars;
				fread(&kerningPars, block.size, 1, pFile);
				break;
			default:
				break;
			}
		}

	} while (false);

	fclose(pFile);

	return true;
}

bool Font::isFnt(const unsigned char* pData, size_t dataSize)
{
	if (dataSize < 4)
		return false;

	static unsigned char FNT_FILETYPE[] = { 'B' ,'M', 'F', 3};

	return memcmp(FNT_FILETYPE, pData, sizeof(FNT_FILETYPE)) == 0;
}

bool Font::readFntBlock(FntBlock* fntBlock, FILE* pFile)
{
	if (fread(fntBlock, sizeof(FntBlock), 1, pFile) == 1)
		return true;

	return false;
}

void Font::readFntInfo(FntBlock& fntBlock, FntInfo& fntInfo, FILE* pFile)
{
	fread(&fntInfo, sizeof(FntInfo), 1, pFile);

	const int length = fntBlock.size - sizeof(FntInfo);
	auto fontName = std::make_unique<char[]>(length);
	fread(fontName.get(), length, 1, pFile);
	m_fontName = fontName.get();
}

void Font::readFntCommon(FntBlock& fntBlock, FntCommon& fntCommon, FILE* pFile)
{
	fread(&fntCommon, fntBlock.size, 1, pFile);
}

void Font::readFntPages(FntBlock& fntBlock, FILE* pFile)
{
	const int length = fntBlock.size;
	auto pageName = std::make_unique<char[]>(length);
	fread(pageName.get(), length, 1, pFile);
	m_pageName = pageName.get();
}

void Font::readFntChars(FntBlock& fntBlock, FILE* pFile)
{
	const int charCount = fntBlock.size / sizeof(FntChars);

	for (int i = 0; i < charCount; i++) {
		FntChars fntChar;
		fread(&fntChar, sizeof(FntChars), 1, pFile);
		m_chars.insert(std::make_pair(fntChar.id, fntChar));
	}
}

void Font::setupTextureAtlas(const std::string& filename)
{
	m_textureAtlas = TextureAtlas::create(filename, m_chars.size());
	
}

NS_OCF_END
