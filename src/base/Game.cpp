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

	// ���̓N���X�����
	delete m_input;
	m_input = nullptr;

	// �V�[�������
	delete m_scene;
	m_scene = nullptr;

	// �t�H���g�����
	delete m_font;
	m_font = nullptr;

	// �e�N�X�`���[�}�l�[�W���[�����
	delete m_textureManager;
	m_textureManager = nullptr;

	// �t�@�C�����[�e�B���e�B�����
	FileUtils::destroyInstance();

	// �����_���[�����
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

	m_scene->update(m_deltaTime);
}

void Game::draw()
{
	m_scene->draw(m_renderer, glm::mat4(1.0f));

	m_pFPSLabel->draw(m_renderer, glm::mat4(1.0f));
	m_renderer->draw();
}

void Game::calculateDeltaTime()
{
	auto now = std::chrono::steady_clock::now();
	m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - m_lastUpdate).count() / 1000000.0f;
	m_lastUpdate = now;
}

void Game::onWindowSize(int width, int height)
{
	// �E�B���h�E�S�̂��r���[�|�[�g�ɐݒ�
	m_renderer->setViewPort(0, 0, width, height);
}

OCF_END
