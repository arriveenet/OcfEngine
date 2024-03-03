#include "Renderer.h"
#include "base/Game.h"
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
	// シェーダーを初期化
	m_shaderManager = ShaderManager::getInstance();

	Vertex3fC3fT2f vertices[4];
	vertices[0].position = { -0.5f, 0.5f, 0.0f };
	vertices[1].position = { 0.5f, 0.5f, 0.0f };
	vertices[2].position = { 0.5f, -0.5f, 0.0f };
	vertices[3].position = { -0.5f, -0.5f, 0.0f };

	vertices[0].color = { 1.0f, 1.0f, 1.0f };
	vertices[1].color = { 1.0f, 1.0f, 1.0f };
	vertices[2].color = { 1.0f, 1.0f, 1.0f };
	vertices[3].color = { 1.0f, 1.0f, 1.0f };

	vertices[0].texCoord = { 0.0f, 0.0f };	// left top
	vertices[1].texCoord = { 1.0f, 0.0f };	// top right
	vertices[2].texCoord = { 1.0f, 1.0f };	// bottom right
	vertices[3].texCoord = { 0.0f, 1.0f };	// bottom left

	unsigned short indices[] = {
	0, 1, 2,
	2, 3, 0
	};

	m_SpriteVerts = new VertexArray();
	m_SpriteVerts->init(vertices, indices, 4, 6);

	return true;
}

void Renderer::destroy()
{
	// シェーダーの後始末
	m_shaderManager->removeAllPrograms();
	m_shaderManager->destroyInstance();
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

void Renderer::addCommand(RenderCommand* command)
{
	m_renderCommands.push_back(command);
}

void Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Scene* scene = Game::getInstance()->getCurrentScene();
	glm::mat4 projection = scene->getDefaultCamera()->getProjectionMatrix();
	//glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f);

	glm::mat4 modelView(1.0f);
	modelView *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	modelView *= glm::scale(glm::mat4(1.0f), glm::vec3(400.0f, 400.0f, 1.0f));


	Program* pProgram = m_shaderManager->getProgram(ProgramType::Basic);

	glEnable(GL_TEXTURE_2D);

	glUseProgram(pProgram->getProgram());

	glBindVertexArray(m_SpriteVerts->getHandle());

	GLint location = glGetUniformLocation(pProgram->getProgram(), "uViewProj");
	glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(projection));

	location = glGetUniformLocation(pProgram->getProgram(), "uWorldTransform");
	glUniformMatrix4fv(location, 1, GL_TRUE, glm::value_ptr(modelView));

	auto texture = Game::getInstance()->getTextureManager()->addImage("..\\assets\\textures\\25_Crono.png");
	texture->setActive();

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

	//glMatrixMode(GL_PROJECTION);
	////Scene* scene = Game::getInstance()->getCurrentScene();
	//glLoadMatrixf(glm::value_ptr(scene->getDefaultCamera()->getProjectionMatrix()));

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	//// Draw sprites
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//for (auto& sprite : m_sprites) {
	//	sprite->draw();
	//}

	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);

	//// Draw labels
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//for (auto& label : m_labels) {
	//	label->draw();
	//}

	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);

}

OCF_END
