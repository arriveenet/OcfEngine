#include "Window.h"
#include "Application.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

OCF_BEGIN

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
	// �L�[�{�[�h�̃R�[���o�b�N��ݒ�
	glfwSetKeyCallback(m_pWindow, ocf::Keyboard::onKeyEvent);
	// �}�E�X�{�^���̃R�[���o�b�N��ݒ�
	glfwSetMouseButtonCallback(m_pWindow, ocf::Mouse::onMouseButton);
	// �E�B���h�E�T�C�Y�ύX���̃R�[���o�b�N��ݒ�
	glfwSetWindowSizeCallback(m_pWindow, ocf::Applicaiton::onWindowSize);
}

void Window::getCursorPos(double& posX, double& posY) const
{
	glfwGetCursorPos(m_pWindow, &posX, &posY);
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

OCF_END
