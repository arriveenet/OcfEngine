#include "Game.h"
#include "2d/Camera.h"
#include "platform/Application.h"
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

	initMatrixStack();

	return true;
}

void Game::mainLoop()
{
	Applicaiton* app = Applicaiton::getInstance();
	m_running = true;

	if (m_scene == nullptr) {
		release();
		return;
	}

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

void Game::runWithScene(Scene* pScene)
{
	m_scene = pScene;
}

glm::vec2 Game::getVisibleSize() const
{
	return Applicaiton::getInstance()->getWindowSize();
}

void Game::initMatrixStack()
{
	while (!m_projectionMatrixStack.empty()) {
		m_projectionMatrixStack.pop();
	}

	while (!m_modelViewMatrixStack.empty()) {
		m_modelViewMatrixStack.pop();
	}

	m_projectionMatrixStack.push(glm::mat4(1.0f));
	m_modelViewMatrixStack.push(glm::mat4(1.0f));
}

void Game::loadIdentityMatrix(MatrixStack type)
{
	switch (type) {
	case MatrixStack::Projection:
		m_projectionMatrixStack.top() = glm::mat4(1.0f);
		break;
	case MatrixStack::ModelView:
		m_modelViewMatrixStack.top() = glm::mat4(1.0f);
		break;
	default:
		assert(false);
		break;
	}
}

void Game::loadMatrix(MatrixStack type, const glm::mat4& matrix)
{
	switch (type) {
	case MatrixStack::Projection:
		m_projectionMatrixStack.top() = matrix;
		break;
	case MatrixStack::ModelView:
		m_modelViewMatrixStack.top() = matrix;
		break;
	default:
		assert(false);
		break;
	}
}

void Game::multiplyMatrix(MatrixStack type, const glm::mat4& matrix)
{
	switch (type) {
	case MatrixStack::Projection:
		m_projectionMatrixStack.top() *= matrix;
		break;
	case MatrixStack::ModelView:
		m_modelViewMatrixStack.top() *= matrix;
		break;
	default:
		assert(false);
		break;
	}
}

void Game::pushMatrix(MatrixStack type)
{
	switch (type) {
	case MatrixStack::Projection:
		m_projectionMatrixStack.push(m_projectionMatrixStack.top());
		break;
	case MatrixStack::ModelView:
		m_modelViewMatrixStack.push(m_modelViewMatrixStack.top());
		break;
	default:
		assert(false);
		break;
	}
}

void Game::popMatrix(MatrixStack type)
{
	switch (type) {
	case MatrixStack::Projection:
		m_projectionMatrixStack.pop();
		break;
	case MatrixStack::ModelView:
		m_modelViewMatrixStack.pop();
		break;
	default:
		assert(false);
		break;
	}
}

const glm::mat4& Game::getMatrix(MatrixStack type)
{
	switch (type) {
	case MatrixStack::Projection:
		return m_projectionMatrixStack.top();
	case MatrixStack::ModelView:
		return m_modelViewMatrixStack.top();
	default:
		assert(false);
	}

	return m_modelViewMatrixStack.top();
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

	// シーンを描画
	m_scene->render(m_renderer, glm::mat4(1.0f));

	// ステータスを描画
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
		prevCalls = currentCalls;
	}

	if (currentVerts != prevVerts) {
		sprintf_s(buffer, "Draw vert: %u", currentVerts);
		m_pDrawVertexLabel->setString(buffer);
		prevVerts = currentVerts;
	}

	m_pFPSLabel->update(m_deltaTime);
	m_pDrawCallLabel->update(m_deltaTime);
	m_pDrawVertexLabel->update(m_deltaTime);

	Camera* pCamera = m_scene->getDefaultCamera();
	glm::mat4 modelView = pCamera->getViewMatrix();

	pushMatrix(MatrixStack::Projection);
	loadMatrix(MatrixStack::Projection, pCamera->getProjectionMatrix());

	m_pFPSLabel->visit(m_renderer, modelView, 0);
	m_pDrawCallLabel->visit(m_renderer, modelView, 0);
	m_pDrawVertexLabel->visit(m_renderer, modelView, 0);

	popMatrix(MatrixStack::Projection);
}

void Game::onWindowSize(int width, int height)
{
	// ウィンドウ全体をビューポートに設定
	m_renderer->setViewPort(0, 0, width, height);
}

OCF_END
