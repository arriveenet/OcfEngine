#include "GLView.h"
#include "base/Game.h"
#include "2d/Camera2D.h"

namespace ocf {

GLContextAttributes GLView::m_glContextAttributes = { 8, 8, 8, 8, 24 };

GLView::GLView()
    : m_windowSize(0.0f, 0.0f)
    , m_designResolutionSize(0.0f, 0.0f)
    , m_viewportRect()
    , m_scaleX(0.0f)
    , m_scaleY(0.0f)
{
}

GLView::~GLView()
{
}

void GLView::pollEvents()
{
}

void GLView::setFrameSize(float widht, float height)
{
    m_windowSize = glm::vec2(widht, height);

    if (m_designResolutionSize == glm::vec2(0, 0)) {
        m_designResolutionSize = m_windowSize;
    }
}

void GLView::setViewport(float x, float y, float w, float h)
{
    glm::vec4 viewport = {};
    viewport.x = x + m_scaleX + m_viewportRect.m_position.x;
    viewport.y = y + m_scaleY + m_viewportRect.m_position.y;
    viewport.z = w * m_scaleX;
    viewport.w = h * m_scaleY;
    Camera2D::setDefaultViewport(viewport);
}

glm::vec2 GLView::getWindowSize() const
{
    return m_windowSize;
}

void GLView::setDesignResolutionSize(float width, float height)
{
    if (width <= 0.0f || height <= 0.0f) {
        return;
    }

    m_designResolutionSize.x = width;
    m_designResolutionSize.y = height;

    updateDesignResolutionSize();
}

const glm::vec2& GLView::getDesignResolutionSize() const
{
    return m_designResolutionSize;
}

void GLView::setViewName(std::string_view viewname)
{
    m_viewName = viewname;
}

std::string_view GLView::getViewName() const
{
    return m_viewName;
}

void GLView::renderScene(Scene* /* scene */, Renderer* /* renderer */)
{
}

void GLView::setGLContextAttributes(GLContextAttributes& glContextAttribues)
{
    m_glContextAttributes = glContextAttribues;
}

GLContextAttributes GLView::getGLContextAttributes()
{
    return m_glContextAttributes;
}

void GLView::updateDesignResolutionSize()
{
    if ((m_windowSize.x <= 0.0f) && (m_windowSize.y <= 0.0f)
        && (m_designResolutionSize.x <= 0.0f) && (m_designResolutionSize.y <= 0.0f)) {
        return;
    }

    m_scaleX = m_windowSize.x / m_designResolutionSize.y;
    m_scaleY = m_windowSize.y / m_designResolutionSize.y;

    m_scaleX = m_scaleY = std::min(m_scaleX, m_scaleY);

    // ビューポートの矩形を設定
    const float viewportWidth = m_designResolutionSize.x * m_scaleX;
    const float viewportHeight = m_designResolutionSize.y * m_scaleY;

    m_viewportRect.setRect((m_windowSize.x - viewportWidth) / 2.0f,
                           (m_windowSize.y - viewportHeight) / 2.0f,
                           viewportWidth, viewportHeight);

    Game* pGame = Game::getInstance();
    pGame->m_resolutionSize = getDesignResolutionSize();
    pGame->setProjection(pGame->getProjection());
}

} // namespace ocf
