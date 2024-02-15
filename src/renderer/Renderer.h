#pragma once
#include "../Sprite.h"
#include <vector>

class Renderer {
	std::vector<Sprite*> m_sprites;

public:
	Renderer();
	~Renderer();

	bool init();
	void destroy();

	void addSprite(Sprite* pSprite);
	void removeSprite(Sprite* pSprite);

	void draw();
};