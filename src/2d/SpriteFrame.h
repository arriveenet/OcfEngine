#pragma once
#include <string>
#include <glm/glm.hpp>
#include "base/GameObject.h"
#include "base/Rect.h"

NS_OCF_BEGIN

class Texture2D;

class SpriteFrame : public GameObject {
public:
	static SpriteFrame* createWithTexture(Texture2D* texture,
										  const Rect& rect,
										  bool rotated,
										  const glm::vec2& offset,
										  const glm::vec2& originalSize);

	SpriteFrame();
	virtual ~SpriteFrame();

	bool initWithTexture(Texture2D* texture, const Rect& rect, bool rotated,
						   const glm::vec2& offset, const glm::vec2& originalSize);

	bool initWithTextureFilename(std::string_view filename, const Rect& rect, bool rotated,
								 const glm::vec2& offset, const glm::vec2& originalSize);

	Texture2D* getTexture() const;

	const glm::vec2& getOriginalSize() const { return m_originalSize; }

	const Rect& getRect() const { return m_rect; }

	bool isRotated() const { return m_rotated; }

protected:
	Texture2D* m_pTexture;
	glm::vec2 m_offset;
	glm::vec2 m_originalSize;
	Rect m_rect;
	bool m_rotated;
	std::string m_textureFilename;
};

NS_OCF_END