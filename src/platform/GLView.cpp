#include "GLView.h"

NS_OCF_BEGIN

GLContextAttributes GLView::m_glContextAttributes = { 8, 8, 8, 8, 24 };

GLView::GLView()
	: m_screenSize(0.0f, 0.0f)
	, m_resolutionSize(0.0f, 0.0f)
{
}

GLView::~GLView()
{
}

void GLView::pollEvents()
{
}

glm::vec2 GLView::getVisibleSize() const
{
	return m_screenSize;
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

NS_OCF_END
