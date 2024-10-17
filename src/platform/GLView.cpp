#include "GLView.h"
#include "base/Game.h"

NS_OCF_BEGIN

GLContextAttributes GLView::m_glContextAttributes = { 8, 8, 8, 8, 24 };

GLView::GLView()
    : m_screenSize(0.0f, 0.0f)
    , m_designResolutionSize(0.0f, 0.0f)
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
    m_screenSize = glm::vec2(widht, height);

    if (m_designResolutionSize == glm::vec2(0, 0)) {
        m_designResolutionSize = m_screenSize;
    }
}

glm::vec2 GLView::getVisibleSize() const
{
    return m_screenSize;
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

void GLView::renderScene(Scene* scene, Renderer* renderer)
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
    Game* pGame = Game::getInstance();
    pGame->m_windowSizeInPoints = getDesignResolutionSize();
    pGame->setProjection(pGame->getProjection());
}

NS_OCF_END
