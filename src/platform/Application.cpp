#include "Application.h"
#include "base/Game.h"
#include "Scene/MainScene.h"

OCF_BEGIN

Applicaiton* g_pApplication = nullptr;

Applicaiton* Applicaiton::getInstance()
{
	if (g_pApplication == nullptr) {
		g_pApplication = new Applicaiton();
		g_pApplication->init();
	}

	return g_pApplication;
}

Applicaiton::Applicaiton()
	: m_window()
	, m_windowWidth(720)
	, m_windowHeight(480)
{
}

Applicaiton::~Applicaiton()
{
}

bool Applicaiton::init()
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_windowHeight = 720;
	m_windowWidth = m_windowHeight * 4 / 3;	

	m_window.create(m_windowWidth, m_windowHeight, "Happy Birthday to oocfuu!");

	const int displayWidth = GetSystemMetrics(SM_CXSCREEN);
	const int displayHeight = GetSystemMetrics(SM_CYSCREEN);
	const int windowPosX = (displayWidth / 2) - (m_windowWidth / 2);
	const int windowPosY = (displayHeight / 2) - (m_windowHeight / 2);

	m_window.setWindowPos(windowPosX, windowPosY);

	m_window.setCallback();

	const int status = gladLoadGL();
	if (status != 1) {
		return false;
	}
	printf("GL %d.%d\n", GLVersion.major, GLVersion.minor);

	glfwSwapInterval(1);

	return true;
}

void Applicaiton::destroy()
{
	m_window.destroy();
	glfwTerminate();

	delete g_pApplication;
	g_pApplication = nullptr;
}

int Applicaiton::run()
{
	auto game = Game::getInstance();

	// Main loop
	game->mainLoop();

	// Destroy application
	destroy();

	return 0;
}

void Applicaiton::exit()
{
	m_window.close();
}

void Applicaiton::swapBuffers()
{
	m_window.swapBuffers();
}

void Applicaiton::pollEvents()
{
	m_window.pollEvents();
}

bool Applicaiton::windowShouldClose()
{
	return m_window.windowShouldClose();
}

double Applicaiton::getTime()
{
	return glfwGetTime();
}

glm::ivec2 Applicaiton::getWindowSize() const
{
	return glm::ivec2(m_windowWidth, m_windowHeight);
}

void Applicaiton::onWindowSize(GLFWwindow* window, int width, int height)
{
	Game::getInstance()->onWindowSize(width, height);
}

OCF_END