#include "Renderer.h"
#include "base/Game.h"
#include "renderer/OpenGLInclude.h"
#include "renderer/OpenGLUtility.h"
#include "renderer/CustomCommand.h"
#include <glm/gtc/type_ptr.hpp>

NS_OCF_BEGIN

#ifdef _DEBUG
/**
 * @brief OpenGLデバッグメッセージのコールバック関数
 * 
 * @param[in] source
 * @param[in] type
 * @param[in] id
 * @param[in] severity
 * @param[in] length
 * @param[in] message
 * @param[in] user_param
 * 
 */
static void debugMessageCallback(GLenum source, GLenum type, GLuint id,
                                    GLenum severity, GLsizei length, 
                                    GLchar const* message, void const* user_param)
{
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        printf("OpenGL: %s\n", message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        printf("OpenGL error: %s\n", message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        printf("OpenGL warning: %s\n", message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        //printf("OpenGL info: %s\n", message);
        break;
    default:
        break;
    }
}
#endif

Renderer::Renderer()
    : m_viewport(0, 0, 0, 0)
    , m_pTriangleBatchToDraw(nullptr)
    , m_triangleVertices()
    , m_triangleIndices()
    , m_triangleVertexCount(0)
    , m_triangleIndexCount(0)
    , m_pVertexArray(nullptr)
    , m_drawCallCount(0)
    , m_drawVertexCount(0)
{
    m_renderGroups.emplace_back();
    m_triangleBatchToDrawSize = 256;
    m_pTriangleBatchToDraw = static_cast<TriangleBatchToDraw*>(std::malloc(sizeof(TriangleBatchToDraw) * m_triangleBatchToDrawSize));
}

Renderer::~Renderer()
{
    destroy();
}

bool Renderer::init()
{
    m_pVertexArray = new VertexArray();
    m_pVertexArray->bind();

    m_pVertexArray->setStride(sizeof(Vertex3fC3fT2f));

    m_pVertexArray->createVertexBuffer(BufferUsage::Dynamic);
    m_pVertexArray->createIndexBuffer(BufferUsage::Dynamic);

    m_pVertexArray->updateVertexBuffer(m_triangleVertices, m_triangleVertexCount);
    m_pVertexArray->updateIndexBuffer(m_triangleIndices, m_triangleIndexCount);

    m_pVertexArray->setAttribute("inPosition", 0, 3, false, 0);
    m_pVertexArray->setAttribute("inNormal", 1, 3, false, sizeof(float) * 3);
    m_pVertexArray->setAttribute("inTexCoord", 2, 2, false, sizeof(float) * 6);

    m_pVertexArray->bindVertexBuffer();

    m_pVertexArray->unbind();

    m_trianglesCommands.reserve(64);

    // アルファブレンディングを有効にする
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // OpenGLのデバッグメッセージを有効にする
#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugMessageCallback, nullptr);
#endif

    return true;
}

void Renderer::destroy()
{
    std::free(m_pTriangleBatchToDraw);

    delete m_pVertexArray;
    m_pVertexArray = nullptr;
}

void Renderer::beginFrame()
{
}

void Renderer::endFrame()
{
    m_drawCallCount = 0;
    m_drawVertexCount = 0;
}

void Renderer::setViewPort(int x, int y, int width, int height)
{
    m_viewport = { x, y, width, height };
    glViewport(x, y, width, height);
}

glm::ivec4 Renderer::getViewport() const
{
    return m_viewport;
}

void Renderer::addCommand(RenderCommand* command)
{
    m_renderGroups[0].emplace_back(command);
}

void Renderer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::clean()
{
    for (auto&& renderQueue : m_renderGroups) {
        renderQueue.clear();
    }

    m_trianglesCommands.clear();
}

void Renderer::draw()
{
    for (auto&& renderQueue : m_renderGroups) {
        renderQueue.sort();
    }
    visitRenderQueue(m_renderGroups[0]);

    clean();
}

void Renderer::flush()
{
    flush2D();
    flush3D();
}

void Renderer::flush2D()
{
    drawTrianglesCommand();
}

void Renderer::flush3D()
{
}

void Renderer::visitRenderQueue(RenderQueue& queue)
{
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_NEG));
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_ZERO));
    doVisitRenderQueue(queue.getSubQueue(RenderQueue::QueueGroup::GLOBALZ_POS));
}

void Renderer::doVisitRenderQueue(const std::vector<RenderCommand*>& renderCommands)
{
    for (const auto& command : renderCommands) {
        processRenderCommand(command);
    }
    flush();
}

void Renderer::processRenderCommand(RenderCommand* command)
{
    const auto commandType = command->getType();

    switch (commandType) {
    case RenderCommand::Type::TrianglesCommand:
    {
        flush3D();

        TrianglesCommand* cmd = static_cast<TrianglesCommand*>(command);

        if ((m_triangleVertexCount + cmd->getVertexCount() > VBO_SIZE)
            || (m_triangleIndexCount + cmd->getIndexCount() > INDEX_VBO_SIZE)) {
            drawTrianglesCommand();
        }

        m_trianglesCommands.emplace_back(cmd);
    }
    break;
    case RenderCommand::Type::CustomCommand:
    {
        flush();
        drawCustomCommand(command);
    }
    break;
    case RenderCommand::Type::MeshCommand:
    {
        flush2D();
        drawMeshCommand(command);
    }
    break;
    default:
        assert(false);
        break;
    }
}

void Renderer::trianglesVerticesAndIndices(TrianglesCommand* pCmd, unsigned int vertexBufferOffset)
{
    // 頂点データを配列に追加
    unsigned int vertexCount = pCmd->getTriangles().vertexCount;
    memcpy(&m_triangleVertices[m_triangleVertexCount], pCmd->getTriangles().vertices, sizeof(Vertex3fC3fT2f) * vertexCount);
    
    // ローカル座標からワールド座標に変換
    const glm::mat4& modelView = pCmd->getModelView();
    for (unsigned int i = 0; i < vertexCount; i++) {
        Vertex3fC3fT2f vertex = m_triangleVertices[m_triangleVertexCount + i];
        m_triangleVertices[m_triangleVertexCount + i].position = modelView * glm::vec4(vertex.position, 1.0f);
    }

    // インデックスを配列に追加
    unsigned short* indices = pCmd->getTriangles().indices;
    unsigned int indexCount = pCmd->getTriangles().indexCount;
    for (unsigned int i = 0; i < indexCount; i++) {
        int a = m_triangleVertexCount + indices[i];
        m_triangleIndices[m_triangleIndexCount + i] = m_triangleVertexCount + indices[i];
    }

    m_triangleVertexCount += vertexCount;
    m_triangleIndexCount += indexCount;
}

void Renderer::drawTrianglesCommand()
{
    if (m_trianglesCommands.empty())
        return;

    /*------------- 1: 頂点とインデックスをセットアップする -------------*/
    m_pTriangleBatchToDraw[0].pCommand = nullptr;
    m_pTriangleBatchToDraw[0].indicesToDraw = 0;
    m_pTriangleBatchToDraw[0].offset = 0;

    m_triangleVertexCount = 0;
    m_triangleIndexCount = 0;
    int batchTotal = 0;
    bool firstCommand = true;

    uint32_t prevMaterialID = 0;

    for (const auto& cmd : m_trianglesCommands) {
        trianglesVerticesAndIndices(cmd, 0);

        uint32_t currentMaterialID = cmd->getMaterialID();

        if ((prevMaterialID == currentMaterialID) || firstCommand) {
            m_pTriangleBatchToDraw[batchTotal].indicesToDraw += cmd->getIndexCount();
            m_pTriangleBatchToDraw[batchTotal].pCommand = cmd;
        } else {
            if (!firstCommand) {
                batchTotal++;
                m_pTriangleBatchToDraw[batchTotal].offset = m_pTriangleBatchToDraw[batchTotal - 1].offset + m_pTriangleBatchToDraw[batchTotal - 1].indicesToDraw;
            }

            m_pTriangleBatchToDraw[batchTotal].pCommand = cmd;
            m_pTriangleBatchToDraw[batchTotal].indicesToDraw = cmd->getIndexCount();
        }

        if (batchTotal + 1 >= m_triangleBatchToDrawSize) {
            m_triangleBatchToDrawSize = static_cast<int>(m_triangleBatchToDrawSize * 1.4);

            m_pTriangleBatchToDraw = static_cast<TriangleBatchToDraw*>(std::realloc(m_pTriangleBatchToDraw, sizeof(TriangleBatchToDraw) * m_triangleBatchToDrawSize));
        }

        prevMaterialID = currentMaterialID;
        firstCommand = false;
    }
    batchTotal++;

    m_pVertexArray->bind();

    m_pVertexArray->updateVertexBuffer(m_triangleVertices, sizeof(m_triangleVertices[0]) * m_triangleVertexCount);
    m_pVertexArray->updateIndexBuffer(m_triangleIndices, sizeof(m_triangleIndices[0]) * m_triangleIndexCount);

    /*------------- 2: 描画 -------------*/
    for (int i = 0; i < batchTotal; i++) {
        auto& drawInfo = m_pTriangleBatchToDraw[i];
        if (drawInfo.pCommand == nullptr)
            continue;

        ProgramState& programState = drawInfo.pCommand->getProgramState();
        Program* pProgram = programState.getProgram();
        pProgram->use();

        char* buffer = programState.getUniformBuffers();
        pProgram->bindUniformBuffers(buffer);

        Texture2D* pTexture = drawInfo.pCommand->getTexture();
        if (pTexture) {
            pTexture->setActive();
        }

        glDrawElements(GL_TRIANGLES, drawInfo.indicesToDraw, GL_UNSIGNED_SHORT, (GLvoid*)(sizeof(m_triangleIndices[0]) * drawInfo.offset));

        m_drawCallCount++;
        m_drawVertexCount += drawInfo.indicesToDraw;
    }
    m_pVertexArray->unbind();

    /*------------- 3: クリーンアップ -------------*/
    m_trianglesCommands.clear();
}

void Renderer::drawCustomCommand(RenderCommand* command)
{
    CustomCommand* cmd = static_cast<CustomCommand*>(command);
    ProgramState& programState = cmd->getProgramState();

    Program* pProgram = programState.getProgram();
    pProgram->use();

    char* buffer = programState.getUniformBuffers();
    pProgram->bindUniformBuffers(buffer);

    cmd->getVertexArray()->bind();

    const auto drawType = cmd->getDrawType();
    if (drawType == CustomCommand::DrawType::Element) {
        glDrawElements(OpenGLUtility::toGLPrimitive(cmd->getPrimitiveType()), 0, GL_UNSIGNED_SHORT, nullptr);
    } else {
        glDrawArrays(OpenGLUtility::toGLPrimitive(cmd->getPrimitiveType()), cmd->getVertexDrawStart(), cmd->getVertexDrawCount());
    }

    m_drawCallCount++;
    m_drawVertexCount += cmd->getVertexDrawCount();

    cmd->getVertexArray()->unbind();
}

void Renderer::drawMeshCommand(RenderCommand* command)
{
    glEnable(GL_DEPTH_TEST);
    drawCustomCommand(command);
    glDisable(GL_DEPTH_TEST);
}

NS_OCF_END
