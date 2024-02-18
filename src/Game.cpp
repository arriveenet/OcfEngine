#include "Game.h"
#include <Windows.h>
#include "Application.h"
#include "scene/MainScene.h"

Game* Game::s_sharedGame = nullptr;

Game::Game()
	: m_running(false)
	, m_pRenderer(nullptr)
	, m_pScene(nullptr)
	, m_input()
{
}

Game::~Game()
{
	// シーンを解放
	delete m_pScene;
	m_pScene = nullptr;

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
	m_pRenderer = new Renderer();
	m_pRenderer->init();

	m_pScene = new MainScene();
	m_pScene->init();

	m_font.init(".\\resource\\Consolas.fnt");

	m_font.setText(500, 0, "Hello World!");

	m_input.init();

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
	static double currentTime;
	Sleep(16);
	float deltaTime = static_cast<float>(Applicaiton::getInstance()->getTime() - currentTime);
	//printf("deltaTime=%f\n", deltaTime);

	processInput();

	currentTime = Applicaiton::getInstance()->getTime();
	m_pScene->update(deltaTime);
}

void Game::draw()
{
	m_pRenderer->draw();
	m_font.draw();
}
