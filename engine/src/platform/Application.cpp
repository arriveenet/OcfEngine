#include "ocf/platform/Application.h"
#include "ocf/core/Engine.h"
#include "ocf/scene/Scene.h"
#include "ocf/platform/PlatformMacros.h"
#include "ocf/platform/RenderView.h"

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

    auto game = Engine::getInstance();
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
