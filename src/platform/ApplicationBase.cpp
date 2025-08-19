#include "ApplicationBase.h"
#include "base/Game.h"

namespace ocf {

ApplicationBase::~ApplicationBase()
{
    Game::destroyInstance();
}

} // namespace ocf
