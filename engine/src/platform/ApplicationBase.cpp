#include "ocf/platform/ApplicationBase.h"
#include "ocf/core/Engine.h"

namespace ocf {

ApplicationBase::~ApplicationBase()
{
    Engine::destroyInstance();
}

} // namespace ocf
