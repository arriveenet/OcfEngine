#include "AppDelegate.h"
#include <base/Game.h>
#include <platform/RenderViewImpl.h>
#include "MainScene.h"

using namespace ocf;;

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
    auto glView = game->getRenderView();
    if (glView == nullptr) {
        glView = RenderViewImpl::createWithRect("Ocf Engine", Rect(0, 0, windowWidth, windowHeight));

        if (glView == nullptr) {
            return false;
        }

        // ウィンドウの位置をディスプレイの中央に設定
        RenderViewImpl* glViewImpl = dynamic_cast<RenderViewImpl*>(glView);
        if (glViewImpl == nullptr) {
            return false;
        }

        auto monitorSize = glViewImpl->getMonitorSize();
        const int windowPosX = static_cast<int>(monitorSize.x / 2) - static_cast<int>(windowWidth / 2);
        const int windowPosY = static_cast<int>(monitorSize.y / 2) - static_cast<int>(windowHeight / 2);
        glViewImpl->setWindowPosition(windowPosX, windowPosY);

        game->setRenderView(glView);
    }

    glView->setDesignResolutionSize(windowWidth, windowHeight);

    MainScene* scene = new MainScene();
    scene->init();
    game->runWithScene(scene);

    return true;
}

