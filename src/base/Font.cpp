#include "Font.h"
#include <memory>
#include <stdio.h>
#include <stdarg.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include "Game.h"

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

	delete m_texture;
	m_texture = nullptr;
}

bool Font::init(const std::string& filename)
{
	if (!parseFntFile(filename)) {
		return false;
	}

	m_texture = new Texture2D();
	if (!m_texture->initWithFile(".\\resource\\Consolas_0.png")) {
		delete m_texture;
		return false;
	}

	vec2 windowSize = Game::getInstance()->getVisibleSize();
	m_projection = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f);

	return true;
}


void Font::setText(float _x, float _y, const char* _format, ...)
{
	va_list ap;
	char str[256];
	char* p;

	va_start(ap, _format);
	vsprintf_s(str, _format, ap);
	va_end(ap);

	vec3 drawPosition = { _x, _y, 0 };

	for (p = str; (*p != '\0') && (*p != '\n'); p++) {
		FntChars& fntChar = m_chars.at(*p);

		FontVertex vertix[4];
		vertix[0].position = drawPosition + vec3(fntChar.xoffset, fntChar.yoffset, 0);
		vertix[0].texCoord = vec2((float)fntChar.x / 256.0f, (float)fntChar.y / 256.0f);
		vertix[1].position = drawPosition + vec3(fntChar.xoffset, fntChar.height + fntChar.yoffset, 0);
		vertix[1].texCoord = vec2((float)fntChar.x / 256.0f, (float)(fntChar.y + fntChar.height) / 256.0f);
		vertix[2].position = drawPosition + vec3(fntChar.width + fntChar.xoffset, fntChar.height + fntChar.yoffset, 0);
		vertix[2].texCoord = vec2((float)(fntChar.x + fntChar.width) / 256.0f, (float)(fntChar.y + fntChar.height) / 256.0f);
		vertix[3].position = drawPosition + vec3(fntChar.width + fntChar.xoffset, fntChar.yoffset, 0);
		vertix[3].texCoord = vec2((float)(fntChar.x + fntChar.width) / 256.0f, (float)fntChar.y / 256.0f);

		int index[] = { 0,1,2,0,2,3 };
		for (int i = 0; i < 6; i++) {
			m_vertices.push_back(vertix[index[i]]);
		}

		drawPosition.x += fntChar.xadvance;
	}
}

const FntChars& Font::getChar(unsigned int id) const
{
	auto iter = m_chars.find(id);
	if (iter != m_chars.end()) {
		return iter->second;
	}

	static FntChars fntChar;
	return fntChar;
}

void Font::draw()
{
	if (m_vertices.empty()) {
		return;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((GLfloat*)&m_projection);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(
		GL_SRC_ALPHA,           // GLenum sfactor
		GL_ONE_MINUS_SRC_ALPHA);// GLenum dfactor

	m_texture->setActive();

	glVertexPointer(3, GL_FLOAT, sizeof(FontVertex), &m_vertices[0].position);
	glTexCoordPointer(2, GL_FLOAT, sizeof(FontVertex), &m_vertices[0].texCoord);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)m_vertices.size());

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glPopMatrix();
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
