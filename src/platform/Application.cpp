#include "Application.h"
#include "base/Game.h"
#include "base/Scene.h"
#include "platform/PlatformMacros.h"
#include "platform/RenderView.h"

namespace ocf {

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

int Applicaiton::run()
{
    if (!applicationDidFinishLaunching()) {
        return 1;
    }

    auto game = Game::getInstance();
    auto glView = game->getRenderView();

    glView->retain();

    // Main loop
    while (!glView->windowShouldClose())
    {
        game->mainLoop();
        glView->pollEvents();
    }

    if (glView->isOpenGLReady()) {
        game->exit();
        game->mainLoop();
        game = nullptr;
    }
    glView->release();

    return 0;
}

} // namespace ocf
