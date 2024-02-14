#include "Renderer.h"
#include "../Game.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::init()
{
	return false;
}

void Renderer::addSprite(Sprite* pSprite)
{
	m_sprites.push_back(pSprite);
}

void Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	Scene* scene = Game::getInstance()->getCurrentScene();
	glLoadMatrixf(glm::value_ptr(scene->m_defaultCamera.getProjectionMatrix()));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (auto& sprite : m_sprites) {
		sprite->draw();
	}
}
