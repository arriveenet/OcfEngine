#include "OpenGLDriver.h"

namespace ocf::backend {

OpenGLDriver* OpenGLDriver::create()
{ 
    OpenGLDriver* driver = new OpenGLDriver();
    return driver;
}

std::string OpenGLDriver::getVenderString() const
{
    return std::string(m_context.state.version);
}

std::string OpenGLDriver::getRendererString() const
{
    return std::string(m_context.state.renderer);
}

} // namespace ocf::backend
