#include "Mesh.h"
#include "base/Game.h"
#include "base/FileUtils.h"
#include "renderer/Renderer.h"
#include "renderer/Texture2D.h"
#include <tiny_obj_loader.h>

NS_OCF_BEGIN

Mesh::Mesh()
    : m_pTexture(nullptr)
{
}

Mesh::~Mesh()
{
}

bool Mesh::setupMesh()
{
    m_meshCommand.setDrawType(CustomCommand::DrawType::Array);
    m_meshCommand.setPrimitiveType(PrimitiveType::Triangle);

    Program* pProgram = ShaderManager::getInstance()->getProgram(ProgramType::Position3D);

    auto& programState = m_meshCommand.getProgramState();
    programState.setProgram(pProgram);

    VertexArray* pVertexArray = m_meshCommand.getVertexArray();
    pVertexArray->bind();

    pVertexArray->setStride(sizeof(glm::vec3));

    pVertexArray->createVertexBuffer(BufferUsage::Static);
    pVertexArray->setAttribute("inPosition", 0, 3, false, 0);
    pVertexArray->bindVertexBuffer();

    pVertexArray->unbind();

    pVertexArray->updateVertexBuffer(m_data.data(), m_data.size() * sizeof(glm::vec3));
    m_meshCommand.setVertexDrawInfo(0, static_cast<unsigned int>(m_data.size()));


    return true;
}

void Mesh::draw(Renderer* renderer, float globalZOrder, const glm::mat4& transform)
{
    glm::mat4 projection = Game::getInstance()->getMatrix(MatrixStack::Projection);
    auto& programState = m_meshCommand.getProgramState();

    programState.setUniform("uMVPMatrix", glm::mat4(1.0f));

    m_meshCommand.init(globalZOrder, transform);
    renderer->addCommand(&m_meshCommand);
}

void Mesh::setTexture(std::string_view fileName)
{
    Texture2D* texture = Game::getInstance()->getTextureManager()->addImage(fileName.data());
    m_pTexture = texture;
}

NS_OCF_END
