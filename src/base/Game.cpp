#include "Game.h"
#include "platform/Application.h"
#include "scene/MainScene.h"
#include "base/FileUtils.h"

#define FPS_UPDATE_INTERVAL	(0.5f)

OCF_BEGIN

Game* Game::s_sharedGame = nullptr;

Game::Game()
	: m_running(false)
	, m_deltaTime(0.0f)
	, m_lastUpdate()
	, m_renderer(nullptr)
	, m_scene(nullptr)
	, m_textureManager(nullptr)
	, m_font(nullptr)
	, m_input(nullptr)
{
}

Game::~Game()
{
	m_pFPSLabel->release();
	m_pDrawCallLabel->release();
	m_pDrawVertexLabel->release();

	// 入力クラスを解放
	delete m_input;
	m_input = nullptr;

	// シーンを解放
	delete m_scene;
	m_scene = nullptr;

	// フォントを解放
	delete m_font;
	m_font = nullptr;

	// テクスチャーマネージャーを解放
	delete m_textureManager;
	m_textureManager = nullptr;

	// ファイルユーティリティを解放
	FileUtils::destroyInstance();

	// レンダラーを解放
	delete m_renderer;
	m_renderer = nullptr;

	s_sharedGame = nullptr;
}

Game* Game::getInstance()
{
	if (!s_sharedGame) {
		s_sharedGame = new Game();
		s_sharedGame->init();
	}

	return s_sharedGame;
}

bool Game::init()
{
	m_lastUpdate = std::chrono::steady_clock::now();

	m_renderer = new Renderer();
	m_renderer->init();

	m_textureManager = new TextureManager();

	m_font = new Font();
	m_font->init("..\\assets\\fonts\\Consolas.fnt");

	m_scene = new MainScene();
	m_scene->init();

	m_input = new Input();
	m_input->init();

	m_pFPSLabel = Label::create("FPS: 0.0");
	m_pDrawCallLabel = Label::create("Draw call: 0");
	m_pDrawVertexLabel = Label::create("Draw vert: 0");

	glm::vec2 visibleSize = getVisibleSize();
	unsigned short fontHeight = m_font->getFntCommon().lineHeight;
	m_pFPSLabel->setPosition(0, visibleSize.y - fontHeight);
	m_pDrawCallLabel->setPosition(0, visibleSize.y - fontHeight * 2);
	m_pDrawVertexLabel->setPosition(0, visibleSize.y - fontHeight* 3);

	return true;
}

void Game::mainLoop()
{
	Applicaiton* app = Applicaiton::getInstance();
	m_running = true;

	while (app->windowShouldClose()) {
		update();
		draw();

		app->pollEvents();
		app->swapBuffers();
	}

	// delete Game instance
	release();
}

void Game::exit()
{
	m_running = false;
	Applicaiton::getInstance()->exit();
}

glm::vec2 Game::getVisibleSize() const
{
	return Applicaiton::getInstance()->getWindowSize();
}

void Game::processInput()
{
	const InputState& inputState = m_input->getState();

	if (inputState.keyboard.getKeyState(GLFW_KEY_ESCAPE) == ButtonState::Pressed) {
		exit();
	}

	m_scene->processInput(inputState);

	m_input->update();
}

void Game::update()
{
	calculateDeltaTime();
	//printf("deltaTime: %f\n", m_deltaTime);

	processInput();

	m_frameRate = 1.0f / m_deltaTime;

	m_scene->update(m_deltaTime);
}

void Game::draw()
{
	m_renderer->beginFrame();

	m_renderer->clear();

	m_scene->draw(m_renderer, glm::mat4(1.0f));

	showStats();
	m_renderer->draw();

	m_renderer->endFrame();
}

void Game::calculateDeltaTime()
{
	auto now = std::chrono::steady_clock::now();
	m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastUpdate).count() / 1000000.0f;
	m_lastUpdate = now;
}

void Game::showStats()
{
	m_frames++;
	m_accumulator += m_deltaTime;

	char buffer[30] = { 0 };

	if (m_pFPSLabel) {
		if (m_accumulator > FPS_UPDATE_INTERVAL) {
			sprintf_s(buffer, "FPS: %.1f", m_frames / m_accumulator);
			m_pFPSLabel->setString(buffer);
			m_pFPSLabel->update(m_deltaTime);

			m_frames = 0;
			m_accumulator = 0.0f;
		}
	}

	static uint32_t prevCalls = 0;
	static uint32_t prevVerts = 0;

	uint32_t currentCalls = m_renderer->getDrawCallCount();
	uint32_t currentVerts = m_renderer->getDrawVertexCount();

	if (currentCalls != prevCalls) {
		sprintf_s(buffer, "Draw call: %u", currentCalls);
		m_pDrawCallLabel->setString(buffer);
		m_pDrawCallLabel->update(m_deltaTime);

		prevCalls = currentCalls;
	}

	if (currentVerts != prevVerts) {
		sprintf_s(buffer, "Draw vert: %u", currentVerts);
		m_pDrawVertexLabel->setString(buffer);
		m_pDrawVertexLabel->update(m_deltaTime);

		prevVerts = currentVerts;
	}

	m_pFPSLabel->draw(m_renderer, glm::mat4(1.0f));
	m_pDrawCallLabel->draw(m_renderer, glm::mat4(1.0f));
	m_pDrawVertexLabel->draw(m_renderer, glm::mat4(1.0f));
}

void Game::onWindowSize(int width, int height)
{
	// ウィンドウ全体をビューポートに設定
	m_renderer->setViewPort(0, 0, width, height);
}

OCF_END
