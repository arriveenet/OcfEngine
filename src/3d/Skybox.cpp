#include "Skybox.h"
#include "2d/Camera2D.h"
#include "renderer/Renderer.h"
#include "renderer/ShaderManager.h"
#include <glm/gtc/matrix_transform.hpp>

namespace {
    float skyboxVertices[] = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
}

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

bool Skybox::init()
{
    return Node::init();
}

bool Skybox::init(std::string_view positive_x,
                  std::string_view negative_x,
                  std::string_view positive_y,
                  std::string_view negative_y,
                  std::string_view positive_z,
                  std::string_view negative_z)
{
    m_textureCube = TextureCube::create(positive_x, negative_x, positive_y, negative_y, positive_z, negative_z);
    if (!m_textureCube)
    {
        return false;
    }

    m_customCommand.setTextureCube(m_textureCube);

    Program* pProgram = ShaderManager::getInstance()->getBuiltinProgram(ProgramType::Skybox);

    auto& programState = m_customCommand.getProgramState();
    programState.setProgram(pProgram);

    m_customCommand.set3D(true);

    initBuffer();

    return true;
}

void Skybox::initBuffer()
{
    m_customCommand.setDrawType(CustomCommand::DrawType::Array);
    m_customCommand.setPrimitiveType(PrimitiveType::Triangle);

    VertexArray* vertexArray = m_customCommand.getVertexArray();
    vertexArray->bind();

    vertexArray->createVertexBuffer(BufferUsage::Static);
    vertexArray->updateVertexBuffer(skyboxVertices, sizeof(skyboxVertices));

    vertexArray->setAttribute("inPosition", 0, 3, false, 0);

    vertexArray->setStride(sizeof(float) * 3);
    vertexArray->bindVertexBuffer();

    vertexArray->unbind();

    m_customCommand.setVertexDrawInfo(0, 36);
}

void Skybox::draw(Renderer* renderer, const glm::mat4& transform)
{
    m_customCommand.init(m_globalZOrder, transform);

    Camera2D* camera = Camera2D::getVisitingCamera();

    glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera->getViewMatrix()));
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 viewProjection = projection * viewMatrix;

    auto& programState = m_customCommand.getProgramState();
    programState.setUniform("uMVPMatrix", &viewProjection, sizeof(viewProjection));

    renderer->addCommand(&m_customCommand);
}

NS_OCF_END
