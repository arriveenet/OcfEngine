#include "Renderer.h"
#include "../Game.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	destroy();
}

bool Renderer::init()
{
	return false;
}

void Renderer::destroy()
{
}

void Renderer::addSprite(Sprite* pSprite)
{
	m_sprites.emplace_back(pSprite);
}

void Renderer::removeSprite(Sprite* pSprite)
{
	auto iter = std::find(m_sprites.begin(), m_sprites.end(), pSprite);
	if (iter != m_sprites.end()) {
		m_sprites.erase(iter);
	}
}

void Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	Scene* scene = Game::getInstance()->getCurrentScene();
	glLoadMatrixf(glm::value_ptr(scene->getDefaultCamera()->getProjectionMatrix()));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnableClientState(GL_VERTEX_ARRAY);
	for (auto& sprite : m_sprites) {
		sprite->draw();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
}
