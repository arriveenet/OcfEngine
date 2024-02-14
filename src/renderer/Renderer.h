#pragma once
#include "../Sprite.h"
#include <vector>

class Renderer {
	std::vector<Sprite*> m_sprites;

public:
	Renderer();
	~Renderer();

	bool init();

	void addSprite(Sprite* pSprite);

	void draw();
};