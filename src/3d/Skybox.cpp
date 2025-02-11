#include "Skybox.h"
#include "2d/Camera.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderManager.h"

NS_OCF_BEGIN

Skybox* Skybox::create(std::string_view positive_x,
                       std::string_view negative_x,
                       std::string_view positive_y,
                       std::string_view negative_y,
                       std::string_view positive_z,
                       std::string_view negative_z)
{
    Skybox* skybox = new Skybox();
    if (skybox->init(positive_x, negative_x, positive_y, negative_y, positive_z, negative_z))
    {
        skybox->autorelease();
        return skybox;
    }

    OCF_SAFE_DELETE(skybox);
    return nullptr;
}

Skybox::Skybox()
    : m_textureCube(nullptr)
{
}

Skybox::~Skybox()
{
    OCF_SAFE_RELEASE(m_textureCube);
}

bool Skybox::init(std::string_view positive_x,
                  std::string_view negative_x,
                  std::string_view positive_y,
                  std::string_view negative_y,
                  std::string_view positive_z,
                  std::string_view negative_z)
{
    m_textureCube = TextureCube::create(positive_x, negative_x, positive_y, negative_y, positive_z, negative_z);

    Program* pProgram = ShaderManager::getInstance()->getBuiltinProgram(ProgramType::Skybox);

    auto& programState = m_customCommand.getProgramState();
    programState.setProgram(pProgram);

    m_customCommand.set3D(true);

    initBuffer();

    return true;
}

void Skybox::initBuffer()
{
    glm::vec3 vertexBuffer[] = {
        glm::vec3( 1, -1, -1),
        glm::vec3( 1,  1, -1),
        glm::vec3(-1,  1, -1),
        glm::vec3(-1, -1, -1),
    };

    unsigned short indexBuffer[] = { 0,1,2,0,2,3 };

    VertexArray* vertexArray = m_customCommand.getVertexArray();
    vertexArray->bind();

    vertexArray->createVertexBuffer(BufferUsage::Static);
    vertexArray->updateVertexBuffer(vertexBuffer, sizeof(vertexBuffer));

    vertexArray->createIndexBuffer(BufferUsage::Static);
    vertexArray->updateIndexBuffer(indexBuffer, sizeof(indexBuffer));

    vertexArray->setAttribute("inPosition", 0, 3, false, 0);

    vertexArray->setStride(sizeof(glm::vec3));
    vertexArray->bindVertexBuffer();

    vertexArray->unbind();

    m_customCommand.setVertexDrawInfo(0, 4);
    m_customCommand.setIndexDrawInfo(0, 6);
}

void Skybox::draw(Renderer* renderer, const glm::mat4& transform)
{
    m_customCommand.init(m_globalZOrder, transform);

    Camera* camera = Camera::getVisitingCamera();
    auto& programState = m_customCommand.getProgramState();
    glm::mat4 projection = camera->getViewProjectionMatrix();
    programState.setUniform("uMVPMatrix", &projection, sizeof(glm::mat4));

    renderer->addCommand(&m_customCommand);
}

NS_OCF_END
