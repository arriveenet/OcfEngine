#pragma once
#include "renderer/OpenGLInclude.h"
#include "base/Macros.h"

OCF_BEGIN

class Window {
public:
	Window();
	virtual ~Window();

	bool create(int width, int height, const char* title);
	void destroy();
	void close();

	void setWindowPos(int posX, int posY);
	void setCallback();
	void getCursorPos(double& posX, double& posY) const;
	bool windowShouldClose();
	void pollEvents();
	void swapBuffers();

private:
	GLFWwindow* m_pWindow;
};

OCF_END
