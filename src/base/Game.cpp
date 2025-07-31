#include "Game.h"
#include "2d/Camera2D.h"
#include "2d/FontManager.h"
#include "2d/Label.h"
#include "2d/SpriteFrameManager.h"
#include "audio/AudioEngine.h"
#include "base/EventDispatcher.h"
#include "base/FileUtils.h"
#include "base/Scene.h"
#include "input/Input.h"
#include "platform/Application.h"
#include "platform/GLView.h"
#include "renderer/Renderer.h"
#include "renderer/TextureManager.h"

#define FPS_UPDATE_INTERVAL	(0.5f)

NS_OCF_BEGIN

Game* Game::s_sharedGame = nullptr;

Game::Game()
    : m_running(false)
    , m_cleanupInNextLoop(false)
    , m_deltaTime(0.0f)
    , m_lastUpdate()
    , m_projection(Projection::_2D)
    , m_windowSize(0.0f, 0.0f)
    , m_resolutionSize(0.0f, 0.0f)
    , m_renderer(nullptr)
    , m_currentScene(nullptr)
    , m_nextScene(nullptr)
    , m_glView(nullptr)
    , m_textureManager(nullptr)
    , m_eventDispatcher(nullptr)
{
}

Game::~Game()
{
    OCF_SAFE_RELEASE(m_pFPSLabel);
    OCF_SAFE_RELEASE(m_pDrawCallLabel);
    OCF_SAFE_RELEASE(m_pDrawVertexLabel);

    // シーンを解放
    OCF_SAFE_DELETE(m_currentScene);

    // テクスチャーマネージャーを解放
    OCF_SAFE_RELEASE(m_textureManager);

    // イベントディスパッチャを解放
    OCF_SAFE_RELEASE(m_eventDispatcher);

    // ファイルユーティリティを解放
    FileUtils::destroyInstance();
    SpriteFrameManager::destroyInstance();
    ShaderManager::destroyInstance();
    FontManager::release();
    AudioEngine::end();

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
    m_running = true;

    m_lastUpdate = std::chrono::steady_clock::now();

    m_renderer = new Renderer();

    m_textureManager = new TextureManager();

    m_eventDispatcher = new EventDispatcher();

    Input::init();

    initMatrixStack();

    return true;
}

void Game::mainLoop()
{
    if (m_cleanupInNextLoop) {
        m_cleanupInNextLoop = false;
        cleanup();
    }
    else {
        update();
        draw();
    }
}

void Game::exit()
{
    m_running = false;
    m_cleanupInNextLoop = true;
}

void Game::cleanup()
{
    OCF_SAFE_DELETE(m_pFPSLabel);
    OCF_SAFE_DELETE(m_pDrawCallLabel);
    OCF_SAFE_DELETE(m_pDrawVertexLabel);

    // シーンを解放
    OCF_SAFE_DELETE(m_currentScene);

    // テクスチャーマネージャーを解放
    OCF_SAFE_DELETE(m_textureManager);

    // イベントディスパッチャを解放
    OCF_SAFE_DELETE(m_eventDispatcher);

    // ファイルユーティリティを解放
    FileUtils::destroyInstance();
    SpriteFrameManager::destroyInstance();
    ShaderManager::destroyInstance();
    FontManager::release();
    AudioEngine::end();

    // レンダラーを解放
    OCF_SAFE_DELETE(m_renderer);

    if (m_glView != nullptr) {
        m_glView->end();
        m_glView = nullptr;
    }
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
        OCF_SAFE_DELETE(m_currentScene);
    }

    m_currentScene = m_nextScene;
    m_currentScene->onEnter();

    m_nextScene = nullptr;
}

glm::vec2 Game::getVisibleSize() const
{
    if (m_glView) {
        return m_glView->getWindowSize();
    }

    return glm::vec2(0, 0);
}

const glm::vec2& Game::getResolutionSize() const
{
    return m_resolutionSize;
}

float Game::getZEye() const
{
    // FOVが60°の場合の距離
    return (m_resolutionSize.y / 1.154700538379252f);	//(2 * tanf(M_PI/6))
}

void Game::setProjection(Projection projection)
{
    const glm::vec2 size = m_resolutionSize;

    if (size.x == 0 || size.y == 0) {
        OCFLOG("failed because size is 0");
        return;
    }

    setViewport();

    switch (projection) {
    case ocf::Game::Projection::_2D:

        break;
    case ocf::Game::Projection::_3D:
        loadIdentityMatrix(MatrixStack::ModelView);
        break;
    default:
        break;
    }

    m_projection = projection;
}

void Game::setViewport()
{
    if (m_glView != nullptr) {
        m_glView->setViewport(0, 0, m_resolutionSize.x, m_resolutionSize.y);
    }
}

void Game::setGLView(GLView* glView)
{
    if (m_glView != glView) {
        m_renderer->init();
        m_glView = glView;
    }
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

void Game::update()
{
    calculateDeltaTime();

    m_frameRate = 1.0f / m_deltaTime;

    if (m_currentScene != nullptr) {
        m_currentScene->update(m_deltaTime);

        Input::update();
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

    if (m_glView) {
        m_glView->swapBuffers();
    }

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
    static bool isCreate = false;
    if (!isCreate) {
        createStatsLabel();
        isCreate = true;
    }

    m_frames++;
    m_accumulator += m_deltaTime;

    char buffer[30] = { 0 };

    if (m_pFPSLabel) {
        if (m_accumulator > FPS_UPDATE_INTERVAL) {
            snprintf(buffer, sizeof(buffer), "FPS: %.1f", static_cast<float>(m_frames) / m_accumulator);
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
        snprintf(buffer, sizeof(buffer), "Draw call: %u", currentCalls);
        m_pDrawCallLabel->setString(buffer);
        prevCalls = currentCalls;
    }

    if (currentVerts != prevVerts) {
        snprintf(buffer, sizeof(buffer), "Draw vert: %u", currentVerts);
        m_pDrawVertexLabel->setString(buffer);
        prevVerts = currentVerts;
    }

    m_pFPSLabel->update(m_deltaTime);
    m_pDrawCallLabel->update(m_deltaTime);
    m_pDrawVertexLabel->update(m_deltaTime);

    Camera2D* pCamera = m_currentScene->getDefaultCamera();
    glm::mat4 modelView = pCamera->getViewMatrix();

    pushMatrix(MatrixStack::Projection);
    loadMatrix(MatrixStack::Projection, pCamera->getProjectionMatrix());

    m_pFPSLabel->visit(m_renderer, modelView, 0);
    m_pDrawCallLabel->visit(m_renderer, modelView, 0);
    m_pDrawVertexLabel->visit(m_renderer, modelView, 0);

    popMatrix(MatrixStack::Projection);
}

void Game::createStatsLabel()
{
    m_pFPSLabel = Label::create("FPS: 0.0");
    m_pDrawCallLabel = Label::create("Draw call: 0");
    m_pDrawVertexLabel = Label::create("Draw vert: 0");

    float fontHeight = m_pFPSLabel->getFont()->getLineHeight();
    m_pFPSLabel->setPosition(glm::vec2(0, 0));
    m_pDrawCallLabel->setPosition(glm::vec2(0, fontHeight));
    m_pDrawVertexLabel->setPosition(glm::vec2(0, fontHeight * 2));
}

NS_OCF_END
