#include "ApplicationBase.h"
#include "base/Game.h"

NS_OCF_BEGIN

ApplicationBase::~ApplicationBase()
{
    Game::destroyInstance();
}

NS_OCF_END
