#include "Mesh.h"
#include "2d/Camera2D.h"
#include "3d/Camera3D.h"
#include "base/Game.h"
#include "base/FileUtils.h"
#include "renderer/Renderer.h"
#include "renderer/Texture2D.h"
#include "renderer/TextureManager.h"
#include <tiny_obj_loader.h>

NS_OCF_BEGIN

Mesh* Mesh::create()
{
    Mesh* mesh = new Mesh();
    mesh->autorelease();
    return mesh;
}

Mesh::Mesh()
    : m_hasNormal(false)
    , m_hasTexCoord(false)
    , m_pTexture(nullptr)
{
}

Mesh::~Mesh()
{
}

bool Mesh::setupMesh()
{
    m_meshCommand.setDrawType(CustomCommand::DrawType::Array);
    m_meshCommand.setPrimitiveType(PrimitiveType::Triangle);

    Program* pProgram = ShaderManager::getInstance()->getBuiltinProgram(ProgramType::Phong);

    auto& programState = m_meshCommand.getProgramState();
    programState.setProgram(pProgram);

    VertexArray* pVertexArray = m_meshCommand.getVertexArray();
    pVertexArray->bind();

    pVertexArray->createVertexBuffer(BufferUsage::Static);

    pVertexArray->updateVertexBuffer(m_data.data(), sizeof(float) * m_data.size());
    m_meshCommand.setVertexDrawInfo(0, static_cast<unsigned int>(m_data.size()));

    size_t stride = 3;
    pVertexArray->setAttribute("inPosition", 0, 3, false, 0);
    if (m_hasNormal) {
        stride += 3;
        pVertexArray->setAttribute("inNormal", 1, 3, false, sizeof(float) * 3);
    }
    if (m_hasTexCoord) {
        stride += 2;
        pVertexArray->setAttribute("inTexCoord", 2, 2, false, sizeof(float) * 6);
    }
    pVertexArray->setStride(sizeof(float) * stride);
    pVertexArray->bindVertexBuffer();

    pVertexArray->unbind();

    return true;
}

void Mesh::draw(Renderer* renderer, float globalZOrder, const glm::mat4& transform)
{
    Camera3D* camera = Camera3D::getVisitingCamera();
    if (camera == nullptr) {
        return;
    }

    glm::mat4 projection = camera->getViewProjectionMatrix();
    auto& programState = m_meshCommand.getProgramState();
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 lightPosition(5.0f, 5.0f, 5.0f);
    glm::vec3 viewPosition(camera->getPosition());

    programState.setUniform("uProjection", &projection, sizeof(projection));
    programState.setUniform("uModelView", &transform, sizeof(transform));
    programState.setUniform("objectColor", &objectColor, sizeof(objectColor));
    programState.setUniform("lightColor", &lightColor, sizeof(lightColor));
    programState.setUniform("lightposition", &lightPosition, sizeof(lightPosition));
    programState.setUniform("viewPosition", &viewPosition, sizeof(viewPosition));

    m_meshCommand.init(globalZOrder);
    renderer->addCommand(&m_meshCommand);
}

void Mesh::setTexture(std::string_view fileName)
{
    Texture2D* texture = Game::getInstance()->getTextureManager()->addImage(fileName.data());
    m_pTexture = texture;
}

NS_OCF_END
