#include "Game.h"
#include <Windows.h>
#include "Application.h"
#include "scene/MainScene.h"

#define FPS_UPDATE_INTERVAL	(0.5f)

OCF_BEGIN

Game* Game::s_sharedGame = nullptr;

Game::Game()
	: m_running(false)
	, m_deltaTime(0.0f)
	, m_lastUpdate()
	, m_pRenderer(nullptr)
	, m_pScene(nullptr)
	, m_pTextureManager(nullptr)
	, m_input()
{
}

Game::~Game()
{
	m_pFPSLabel->release();

	// シーンを解放
	delete m_pScene;
	m_pScene = nullptr;

	// テクスチャーマネージャーを解放
	delete m_pTextureManager;
	m_pTextureManager = nullptr;

	// レンダラーを解放
	delete m_pRenderer;
	m_pRenderer = nullptr;

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

	m_pRenderer = new Renderer();
	m_pRenderer->init();

	m_pTextureManager = new TextureManager();

	m_font.init(".\\resource\\Consolas.fnt");

	m_pScene = new MainScene();
	m_pScene->init();

	m_input.init();

	m_pFPSLabel = Label::create("FPS: 0.0");

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

glm::vec2 Game::getVisibleSize()
{
	return Applicaiton::getInstance()->getWindowSize();
}

void Game::processInput()
{
	const InputState& inputState = m_input.getState();

	if (inputState.keyboard.getKeyState(GLFW_KEY_ESCAPE) == ButtonState::Pressed) {
		exit();
	}

	m_pScene->processInput(inputState);

	m_input.update();
}

void Game::update()
{
	calculateDeltaTime();
	//printf("deltaTime: %f\n", m_deltaTime);

	m_frames++;
	m_accumulator += m_deltaTime;

	if (m_pFPSLabel) {
		if (m_accumulator > FPS_UPDATE_INTERVAL) {
			char buffer[30] = { 0 };
			sprintf_s(buffer, "FPS: %.1f", m_frames / m_accumulator);
			m_pFPSLabel->setString(buffer);
			m_pFPSLabel->update(m_deltaTime);

			m_frames = 0;
			m_accumulator = 0.0f;
		}
	}

	processInput();

	m_frameRate = 1.0f / m_deltaTime;

	m_pScene->update(m_deltaTime);
}

void Game::draw()
{
	m_pRenderer->draw();
}

void Game::calculateDeltaTime()
{
	auto now = std::chrono::steady_clock::now();
	m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastUpdate).count() / 1000000.0f;
	m_lastUpdate = now;
}

OCF_END
