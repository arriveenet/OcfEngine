#include "AppDelegate.h"
#include <base/Game.h>
#include <platform/GLViewImpl.h>
#include "MainScene.h"

USING_NS_OCF;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	auto game = Game::getInstance();
	auto glView = game->getGLView();
	if (glView == nullptr) {
		glView = GLViewImpl::createWithRect("Ocf Engine", Rect(0, 0, 720, 480));
		game->setGLView(glView);
	}

	// @TODO ƒtƒHƒ“ƒg‰Šú‰»ˆ—‚ðŒ©’¼‚·
	game->getFont()->init("fonts\\Consolas.fnt");

	MainScene* scene = new MainScene();
	scene->init();
	game->runWithScene(scene);

	return true;
}

