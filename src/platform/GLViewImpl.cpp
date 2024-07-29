#include "GLViewImpl.h"
#include "input/Mouse.h"
#include <windows.h>

NS_OCF_BEGIN

class GLFWEventHandler {
public:
    static void setGLViewImpl(GLViewImpl* view) { m_view = view; }

    static void onGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (m_view) {
            m_view->onGLFWMouseButtonCallback(window, button, action, mods);
        }
    }

    static void onGLFWMouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if (m_view) {
            m_view->onGLFWMouseMoveCallback(window, xpos, ypos);
        }
    }

    static void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (m_view) {
            m_view->onGLFWKeyCallback(window, key, scancode, action, mods);
        }
    }

    static  void onGLFWWindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        if (m_view) {
            m_view->onGLFWWindowSizeCallback(window, width, height);
        }
    }
private:
    static GLViewImpl* m_view;
};
GLViewImpl* GLFWEventHandler::m_view = nullptr;

GLViewImpl* GLViewImpl::create(std::string_view viewName)
{
    return GLViewImpl::create(viewName, false);
}

GLViewImpl* GLViewImpl::create(std::string_view viewName, bool resizable)
{
    auto result = new GLViewImpl();
    if (result->initWithRect(viewName, Rect(0, 0, 640, 480), resizable)) {
        result->autorelease();
        return result;
    }
    OCF_SAFE_DELETE(result);
    return nullptr;
}

GLViewImpl* GLViewImpl::createWithRect(std::string_view viewName, const Rect& rect, bool resizable)
{
    auto result = new GLViewImpl();
    if (result->initWithRect(viewName, rect, resizable)) {
        result->autorelease();
        return result;
    }
    OCF_SAFE_DELETE(result);
    return nullptr;
}

GLViewImpl::GLViewImpl(bool initGlfw)
    : m_pMainWindow(nullptr)
    , m_mousePosition(0, 0)
{
    GLFWEventHandler::setGLViewImpl(this);
    if (initGlfw) {
        glfwInit();
    }
}

GLViewImpl::~GLViewImpl()
{
    GLFWEventHandler::setGLViewImpl(nullptr);
    glfwDestroyWindow(m_pMainWindow);
    glfwTerminate();
}

bool GLViewImpl::windowShouldClose()
{
    if (m_pMainWindow) {
        return glfwWindowShouldClose(m_pMainWindow) ? true : false;
    }
    else {
        return false;
    }
}

void GLViewImpl::pollEvents()
{
    glfwPollEvents();
}

bool GLViewImpl::isOpenGLReady()
{
    return m_pMainWindow != nullptr;
}

void GLViewImpl::swapBuffers()
{
    glfwSwapBuffers(m_pMainWindow);
}

bool GLViewImpl::initWithRect(std::string_view viewName, const Rect& rect, bool resizable)
{
    setViewName(viewName);

    glm::vec2 windowSize = rect.m_size;
    m_screenSize = windowSize;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, resizable ? GL_TRUE : GL_FALSE);

    m_pMainWindow = glfwCreateWindow(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), 
                                     m_viewName.c_str(), nullptr, nullptr);

    if (m_pMainWindow == nullptr) {
        return false;
    }

    glfwMakeContextCurrent(m_pMainWindow);

#ifdef _WIN32
    const int displayWidth = GetSystemMetrics(SM_CXSCREEN);
    const int displayHeight = GetSystemMetrics(SM_CYSCREEN);
    const int windowPosX = static_cast<int>((displayWidth / 2.0f) - (windowSize.x / 2.0f));
    const int windowPosY = static_cast<int>((displayHeight / 2.0f) - (windowSize.y / 2.0f));
    glfwSetWindowPos(m_pMainWindow, windowPosX, windowPosY);
#endif

    glfwSetMouseButtonCallback(m_pMainWindow, GLFWEventHandler::onGLFWMouseButtonCallback);
    glfwSetCursorPosCallback(m_pMainWindow, GLFWEventHandler::onGLFWMouseMoveCallback);
    glfwSetKeyCallback(m_pMainWindow, GLFWEventHandler::onGLFWKeyCallback);
    glfwSetWindowSizeCallback(m_pMainWindow, GLFWEventHandler::onGLFWWindowSizeCallback);

    const int status = gladLoadGL();
    if (status != 1) {
        return false;
    }

    glfwSwapInterval(1);

    return true;
}

void GLViewImpl::onGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    Mouse::onMouseButton(button, action, mods);
}

void GLViewImpl::onGLFWMouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
    m_mousePosition.x = static_cast<float>(xpos);
    m_mousePosition.y = static_cast<float>(m_screenSize.y - ypos);
}

void GLViewImpl::onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

void GLViewImpl::onGLFWWindowSizeCallback(GLFWwindow* window, int width, int height)
{
}

NS_OCF_END
