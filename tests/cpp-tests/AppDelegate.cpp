#include "AppDelegate.h"
#include <base/Game.h>
#include "MainScene.h"

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
	if (!init()) {
		return false;
	}

	ocf::MainScene* scene = new ocf::MainScene();
	scene->init();
	ocf::Game::getInstance()->runWithScene(scene);

	return true;
}

