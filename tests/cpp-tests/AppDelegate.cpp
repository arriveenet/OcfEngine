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
    const float windowWidth = 920, windowHeight = 720;

    auto game = Game::getInstance();
    auto glView = game->getGLView();
    if (glView == nullptr) {
        glView = GLViewImpl::createWithRect("Ocf Engine", Rect(0, 0, windowWidth, windowHeight));

        if (glView == nullptr) {
            return false;
        }

        // ウィンドウの位置をディスプレイの中央に設定
#ifdef _WIN32
        const int displayWidth = GetSystemMetrics(SM_CXSCREEN);
        const int displayHeight = GetSystemMetrics(SM_CYSCREEN);
        const int windowPosX = static_cast<int>((displayWidth / 2.0f) - (windowWidth / 2.0f));
        const int windowPosY = static_cast<int>((displayHeight / 2.0f) - (windowHeight / 2.0f));
        dynamic_cast<GLViewImpl*>(glView)->setWindowPosition(windowPosX, windowPosY);
#endif
        game->setGLView(glView);
    }

    glView->setDesignResolutionSize(windowWidth, windowHeight);

    MainScene* scene = new MainScene();
    scene->init();
    game->runWithScene(scene);

    return true;
}

