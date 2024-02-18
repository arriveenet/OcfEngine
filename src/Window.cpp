#include "Window.h"
#include "Application.h"
#include "base/Keyboard.h"

Window::Window()
	: m_pWindow(nullptr)
{
}

Window::~Window()
{
}

bool Window::create(int width, int height, const char* title)
{
	m_pWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (m_pWindow == nullptr)
		return false;

	glfwMakeContextCurrent(m_pWindow);

	return true;
}

void Window::destroy()
{
	glfwDestroyWindow(m_pWindow);
}

void Window::close()
{
	glfwSetWindowShouldClose(m_pWindow, GL_TRUE);
}

void Window::setWindowPos(int posX, int posY)
{
	glfwSetWindowPos(m_pWindow, posX, posY);
}

void Window::setCallback()
{
	glfwSetKeyCallback(m_pWindow, Keyboard::onKeyEvent);
}

bool Window::windowShouldClose()
{
	return (glfwWindowShouldClose(m_pWindow) == 0) ? true : false;
}

void Window::pollEvents()
{
	glfwPollEvents();
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}
