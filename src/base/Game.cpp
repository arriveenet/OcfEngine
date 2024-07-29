#include "Game.h"
#include "2d/Camera.h"
#include "2d/SpriteFrameManager.h"
#include "platform/Application.h"
#include "base/FileUtils.h"

#define FPS_UPDATE_INTERVAL	(0.5f)

NS_OCF_BEGIN

Game* Game::s_sharedGame = nullptr;

Game::Game()
	: m_running(false)
	, m_deltaTime(0.0f)
	, m_lastUpdate()
	, m_renderer(nullptr)
	, m_currentScene(nullptr)
	, m_nextScene(nullptr)
	, m_textureManager(nullptr)
	, m_font(nullptr)
	, m_input(nullptr)
{
}

Game::~Game()
{
	OCF_SAFE_RELEASE(m_pFPSLabel);
	OCF_SAFE_RELEASE(m_pDrawCallLabel);
	OCF_SAFE_RELEASE(m_pDrawVertexLabel);

	// 入力クラスを解放
	OCF_SAFE_DELETE(m_input);

	// シーンを解放
	OCF_SAFE_RELEASE(m_currentScene);

	// フォントを解放
	OCF_SAFE_DELETE(m_font);

	// テクスチャーマネージャーを解放
	OCF_SAFE_DELETE(m_textureManager);

	// ファイルユーティリティを解放
	FileUtils::destroyInstance();
	SpriteFrameManager::destroyInstance();

	// レンダラーを解放
	OCF_SAFE_DELETE(m_renderer);

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

void Game::destroyInstance()
{
	OCF_SAFE_DELETE(s_sharedGame);
}

bool Game::init()
{
	m_lastUpdate = std::chrono::steady_clock::now();

	m_renderer = new Renderer();
	m_renderer->init();

	m_textureManager = new TextureManager();

	m_font = new Font();
	m_font->init("fonts\\Consolas.fnt");

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
	update();
	draw();
}

void Game::exit()
{
	m_running = false;
	Applicaiton::getInstance()->exit();
}

void Game::runWithScene(Scene* pScene)
{
	pushScene(pScene);
}

void Game::replaceScene(Scene* pScene)
{
	assert(pScene != nullptr);

	if (m_currentScene == nullptr) {
		runWithScene(pScene);
		return;
	}

	const size_t index = m_sceneStack.size() - 1;
	m_sceneStack[index] = pScene;

	m_nextScene = pScene;
}

void Game::pushScene(Scene* pScene)
{
	m_sceneStack.emplace_back(pScene);
	m_nextScene = pScene;
}

void Game::popScene()
{
	m_sceneStack.pop_back();
}

void Game::setNextScene()
{
	if (m_currentScene != nullptr) {
		m_currentScene->onExit();
		m_currentScene->release();
	}

	m_currentScene = m_nextScene;
	m_currentScene->onEnter();

	m_nextScene = nullptr;
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

	m_currentScene->processInput(inputState);

	m_input->update();
}

void Game::update()
{
	calculateDeltaTime();

	m_frameRate = 1.0f / m_deltaTime;

	if (m_currentScene != nullptr) {
		processInput();

		m_currentScene->update(m_deltaTime);
	}
}

void Game::draw()
{
	m_renderer->beginFrame();

	m_renderer->clear();

	if (m_nextScene != nullptr) {
		setNextScene();
	}

	pushMatrix(MatrixStack::ModelView);

	// シーンを描画
	if (m_currentScene != nullptr) {
		m_currentScene->render(m_renderer, glm::mat4(1.0f));
	}

	// ステータスを描画
	showStats();
	m_renderer->draw();

	popMatrix(MatrixStack::ModelView);

	Applicaiton::getInstance()->swapBuffers();

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

	Camera* pCamera = m_currentScene->getDefaultCamera();
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

NS_OCF_END
