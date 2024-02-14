#include "Game.h"
#include "Application.h"
#include "scene/MainScene.h"
#include <Windows.h>

Game::Game()
	: m_running(false)
{
}

Game::~Game()
{
}

Game* Game::getInstance()
{
	static Game game;
	return &game;
}

bool Game::init()
{
	m_pRenderer = new Renderer();
	m_pRenderer->init();

	m_pScene = new MainScene();
	m_pScene->init();

	return true;
}

void Game::destroy()
{
	delete m_pRenderer;
	m_pRenderer = nullptr;

	delete m_pScene;
	m_pScene = nullptr;
}

void Game::mainLoop()
{
	Applicaiton* app = Applicaiton::getInstance();
	m_running = true;

	while (app->shouldClose()) {
		update();
		draw();

		app->pollEvents();
		app->swapBuffers();
	}

	destroy();
}

void Game::exit()
{
	m_running = false;
}

void Game::update()
{
	static double currentTime;
	Sleep(16);
	float deltaTime = static_cast<float>(Applicaiton::getInstance()->getTime() - currentTime);
	//printf("deltaTime=%f\n", deltaTime);

	currentTime = Applicaiton::getInstance()->getTime();

	m_pScene->update(deltaTime);
}

void Game::draw()
{
	m_pRenderer->draw();
}
