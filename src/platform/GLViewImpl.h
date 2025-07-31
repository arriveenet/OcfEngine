#include "platform/GLView.h"
#include "input/Input.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

NS_OCF_BEGIN

class GLFWEventHandler;
class GLViewImpl : public GLView {
    friend GLFWEventHandler;

public:
    static GLViewImpl* create(std::string_view viewName);
    static GLViewImpl* create(std::string_view viewName, bool resizable);
    static GLViewImpl* createWithRect(std::string_view viewName, const Rect& rect, bool resizable = false);

    void end() override;

    bool windowShouldClose() override;
    void pollEvents() override;

    bool isOpenGLReady() override;
    void swapBuffers() override;

    void setWindowPosition(int xpos, int ypos);
    void setWindowSize(int* width, int* height);

    int getMonitorCount() const;
    glm::ivec2 getMonitorSize() const;

    glm::vec2 getMousePosition() const { return m_mousePosition; }
    void setCursolPosition(float x, float y);

    void setCursolMode(Input::MouseMode mode);

#if (OCF_TARGET_PLATFORM == OCF_PLATFORM_WIN32)
    HWND getWin32Window() override;
#endif

protected:
    GLViewImpl(bool initGlfw = true);
    virtual ~GLViewImpl();

    bool initWithRect(std::string_view viewName, const Rect& rect, bool resizable);

    void handleWindowSize(int width, int height);

    void onGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void onGLFWMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
    void onGLFWScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onGLFWWindowSizeCallback(GLFWwindow* window, int width, int height);

protected:
    GLFWwindow* m_pMainWindow;
    glm::vec2 m_mousePosition;
    glm::vec2 m_lastMousePosition;
};

NS_OCF_END
