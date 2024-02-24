#include "Renderer.h"
#include "Game.h"
#include "renderer/OpenGLInclude.h"
#include <glm/gtc/type_ptr.hpp>

OCF_BEGIN

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
	const int myDrawOrder = pSprite->getDrawOrder();
	auto iter = m_sprites.begin();
	for (; iter != m_sprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->getDrawOrder()) {
			break;
		}
	}

	m_sprites.insert(iter, pSprite);
}

void Renderer::removeSprite(Sprite* pSprite)
{
	auto iter = std::find(m_sprites.begin(), m_sprites.end(), pSprite);
	if (iter != m_sprites.end()) {
		m_sprites.erase(iter);
	}
}

void Renderer::addLabel(Label* pLabel)
{
	m_labels.push_back(pLabel);
}

void Renderer::setViewPort(int x, int y, int width, int height)
{
	m_viewport = { x, y, width, height };
	glViewport(x, y, width, height);
}

glm::ivec4 Renderer::getViewport() const
{
	return m_viewport;
}

void Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	Scene* scene = Game::getInstance()->getCurrentScene();
	glLoadMatrixf(glm::value_ptr(scene->getDefaultCamera()->getProjectionMatrix()));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw sprites
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& sprite : m_sprites) {
		sprite->draw();
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// Draw labels
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& label : m_labels) {
		label->draw();
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

}

OCF_END
