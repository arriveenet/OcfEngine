#pragma once
#include "GameObject.h"
#include "base/types.h"
#include <glm/glm.hpp>
#include <string>

OCF_BEGIN

class Texture2D : public GameObject {
public:
	Texture2D();
	virtual ~Texture2D();

	bool initWithFile(const std::string& filename);
	void setActive() const;

	int getWidth() const;
	int getHeight() const;

private:
	unsigned int m_textureId;
	int m_width;
	int m_height;
	PixelFormat m_pixelFormat;
	glm::vec2 m_size;
};

OCF_END
