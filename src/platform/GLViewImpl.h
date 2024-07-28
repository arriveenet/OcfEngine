#include "platform/GLView.h"
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

	bool windowShouldClose() override;
	void pollEvents() override;

	bool isOpenGLReady() override;
	void swapBuffers() override;

	glm::vec2 getMousePosition() { return m_mousePosition; }

protected:
	GLViewImpl(bool initGlfw = true);
	virtual ~GLViewImpl();

	bool initWithRect(std::string_view viewName, const Rect& rect, bool resizable);

	void onGLFWMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	void onGLFWMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void onGLFWWindowSizeCallback(GLFWwindow* window, int width, int height);

protected:
	GLFWwindow* m_pMainWindow;
	glm::vec2 m_mousePosition;
};

NS_OCF_END