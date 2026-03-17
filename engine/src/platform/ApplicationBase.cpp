#include "ocf/platform/ApplicationBase.h"
#include "ocf/core/Game.h"

namespace ocf {

ApplicationBase::~ApplicationBase()
{
    Game::destroyInstance();
}

} // namespace ocf
