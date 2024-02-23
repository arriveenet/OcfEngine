#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "renderer/Texture2D.h"
#include"renderer/TextureAtlas.h"
#include "fnt.h"

OCF_BEGIN

class Font : GameObject {
public:
	Font();
	virtual ~Font();

	bool init(const std::string& filename);

	void setText(float _x, float _y, const char* _format, ...);

	const FntChars& getChar(unsigned int id) const;
	const FntInfo& getFntInfo() const {return m_fntInfo; }
	const FntCommon& getFntCommon() const { return m_fntCommon; }

	Texture2D* getTexture() const { return m_texture; }

	void draw();

protected:
	bool parseFntFile(const std::string& fntFile);
	bool isFnt(const unsigned char* pData, size_t dataSize);
	bool readFntBlock(FntBlock* fntBlock, FILE* pFile);
	void readFntInfo(FntBlock& fntBlock, FntInfo& fntInfo, FILE* pFile);
	void readFntCommon(FntBlock& fntBlock, FntCommon& fntCommon, FILE* pFile);
	void readFntPages(FntBlock& fntBlock, FILE* pFile);
	void readFntChars(FntBlock& fntBlock, FILE* pFile);

	void setupTextureAtlas(const std::string& filename);

private:
	struct FontVertex {
		glm::vec3 position;
		glm::vec2 texCoord;
	};
	
	glm::mat4 m_projection;
	Texture2D* m_texture;
	TextureAtlas* m_textureAtlas = nullptr;
	FntInfo m_fntInfo;
	FntCommon m_fntCommon;
	std::string m_fontName;
	std::string m_pageName;
	std::unordered_map<unsigned int, FntChars> m_chars;
	std::vector<FontVertex> m_vertices;
	std::vector<QuadV3fT2f> m_quads;
};

OCF_END
