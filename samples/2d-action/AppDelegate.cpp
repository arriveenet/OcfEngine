#include "AppDelegate.h"
#include <base/Game.h>
#include <platform/GLViewImpl.h>
#include "MainScene.h"
#include <filesystem>

namespace fs = std::filesystem;

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

    auto engine = Game::getInstance();
    auto glView = engine->getGLView();
    if (glView == nullptr) {
        glView = GLViewImpl::createWithRect("2d-action", Rect(0, 0, windowWidth, windowHeight));

        if (glView == nullptr) {
            return false;
        }

        // ウィンドウの位置をディスプレイの中央に設定
        GLViewImpl* glViewImpl = dynamic_cast<GLViewImpl*>(glView);
        if (glViewImpl == nullptr) {
            return false;
        }

        auto monitorSize = glViewImpl->getMonitorSize();
        const int windowPosX = static_cast<int>(monitorSize.x / 2) - static_cast<int>(windowWidth / 2);
        const int windowPosY = static_cast<int>(monitorSize.y / 2) - static_cast<int>(windowHeight / 2);
        glViewImpl->setWindowPosition(windowPosX, windowPosY);

        engine->setGLView(glView);
    }

    glView->setDesignResolutionSize(windowWidth, windowHeight);

    std::string searchPath;

    fs::path currentPath = fs::current_path();
    fs::path rootPath = currentPath.root_path();

    while (currentPath != rootPath) {
        fs::path assetsPath = currentPath;
        assetsPath.append("OcfEngine");
        if (fs::exists(assetsPath)) {
            searchPath = assetsPath.string();
            break;
        }

        currentPath = currentPath.parent_path();
    }
    
    searchPath.append("/samples/2d-action/assets");
    FileUtils::getInstance()->addSearchPath(searchPath, true);

    MainScene* scene = new MainScene();
    scene->init();
    engine->runWithScene(scene);

    return true;
}

