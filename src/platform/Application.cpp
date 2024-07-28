#include "Application.h"
#include "base/Game.h"
#include "2d/Scene.h"
#include "platform/PlatformMacros.h"
#include "platform/GLView.h"

NS_OCF_BEGIN

Applicaiton* g_pApplication = nullptr;

Applicaiton* Applicaiton::getInstance()
{
	assert(g_pApplication);
	return g_pApplication;
}

Applicaiton::Applicaiton()
	: m_windowWidth(720)
	, m_windowHeight(480)
{
	g_pApplication = this;
}

Applicaiton::~Applicaiton()
{
	assert(this == g_pApplication);
	g_pApplication = nullptr;
}

bool Applicaiton::init()
{
	return true;
}

void Applicaiton::destroy()
{
}

int Applicaiton::run()
{
	if (!applicationDidFinishLaunching()) {
		return 1;
	}

	auto game = Game::getInstance();
	auto glView = game->getGLView();

	// Main loop
	while (!glView->windowShouldClose())
	{
		game->mainLoop();
		glView->pollEvents();
	}

	// Destroy application
	destroy();

	glView->release();

	return 0;
}

void Applicaiton::exit()
{

}

NS_OCF_END
