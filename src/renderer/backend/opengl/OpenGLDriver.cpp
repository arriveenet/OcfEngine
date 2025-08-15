#include "OpenGLDriver.h"
#include "OpenGLUtility.h"

namespace ocf::backend {

OpenGLDriver::OpenGLDriver()
{
}

OpenGLDriver::~OpenGLDriver()
{

}

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

TextureHandle OpenGLDriver::createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                          uint32_t width, uint32_t height, uint32_t depth)
{
    GLenum internalFormat = OpenGLUtility::getInternalFormat(format);
    GLTexture t;
    glGenTextures(1, &t.gl.id);
    t.gl.target = OpenGLUtility::getTextureTarget(target);
    //glTexImage2D();
    return TextureHandle();
}

} // namespace ocf::backend
