#include "Renderer.h"
#include "base/Game.h"
#include "renderer/OpenGLInclude.h"
#include "renderer/OpenGLUtility.h"
#include "renderer/CustomCommand.h"
#include <glm/gtc/type_ptr.hpp>

OCF_BEGIN

#ifdef _DEBUG
/**
 * @brief OpenGL�f�o�b�O���b�Z�[�W�̃R�[���o�b�N�֐�
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
	, m_shaderManager(nullptr)
	, m_pTriangleBatchToDraw(nullptr)
	, m_triangleBatchToDrawSize(256)
	, m_triangleVertices()
	, m_triangleIndices()
	, m_triangleVertexCount(0)
	, m_triangleIndexCount(0)
	, m_pVertexBuffer(nullptr)
	, m_pIndexBuffer(nullptr)
{
}

Renderer::~Renderer()
{
	destroy();
}

bool Renderer::init()
{
	// �V�F�[�_�[��������
	m_shaderManager = ShaderManager::getInstance();

	m_pTriangleBatchToDraw = static_cast<TriangleBatchToDraw*>(std::malloc(sizeof(TriangleBatchToDraw) * m_triangleBatchToDrawSize));

	m_pVertexBuffer = new VertexBuffer(BufferType::Vertex, BufferUsage::Dynamic);
	m_pIndexBuffer = new VertexBuffer(BufferType::Index, BufferUsage::Dynamic);

	m_trianglesCommands.reserve(64);

	// OpenGL�̃f�o�b�O���b�Z�[�W��L���ɂ���
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugMessageCallback, nullptr);
#endif

	return true;
}

void Renderer::destroy()
{
	// �V�F�[�_�[�̌�n��
	m_shaderManager->removeAllPrograms();
	m_shaderManager->destroyInstance();

	std::free(m_pTriangleBatchToDraw);

	delete m_pVertexBuffer;
	m_pVertexBuffer = nullptr;

	delete m_pIndexBuffer;
	m_pIndexBuffer = nullptr;
}

void Renderer::addSprite(Sprite* pSprite)
{
	const int myDrawOrder = pSprite->getDrawOrder();
	auto iter = m_sprites.begin();
	for (; iter != m_sprites.end(); ++iter) {
		if (myDrawOrder < (*iter)->getDrawOrder()) {
			break;
		}
	}

	m_sprites.insert(iter, pSprite);
}

void Renderer::removeSprite(Sprite* pSprite)
{
	auto iter = std::find(m_sprites.begin(), m_sprites.end(), pSprite);
	if (iter != m_sprites.end()) {
		m_sprites.erase(iter);
	}
}

void Renderer::addLabel(Label* pLabel)
{
	m_labels.push_back(pLabel);
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
	m_renderCommands.push_back(command);
}

void Renderer::draw()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw sprites
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& sprite : m_sprites) {
		sprite->draw();
	}

	for (auto& lable : m_labels) {
		lable->draw();
	}
	glDisable(GL_BLEND);

	for (const auto& command : m_renderCommands) {

		const auto commandType = command->getType();

		switch (commandType) {
		case RenderCommand::Type::TrianglesCommand:
		{
			TrianglesCommand* cmd = static_cast<TrianglesCommand*>(command);
			m_trianglesCommands.emplace_back(cmd);
		}
		break;
		case RenderCommand::Type::CustomCommand:
		{
			flush();
			drawCustomCommand(command);
		}
			break;
		default:
			assert(false);
			break;
		}
	}

	flush();

	m_renderCommands.clear();
}

void Renderer::flush()
{
	drawTrianglesCommand();
}

void Renderer::trianglesVerticesAndIndices(TrianglesCommand* pCmd, unsigned int vertexBufferOffset)
{
	size_t vertexCount = pCmd->getTriangles().vertexCount;
	memcpy(&m_triangleVertices[m_triangleVertexCount], pCmd->getTriangles().vertices, sizeof(Vertex3fC3fT2f) * vertexCount);
	
	const glm::mat4& modelView = pCmd->getModelView();
	for (int i = 0; i < vertexCount; i++) {
		Vertex3fC3fT2f vertex = m_triangleVertices[m_triangleVertexCount + i];
		glm::vec4 pos = modelView * glm::vec4(vertex.position, 1.0f);
		m_triangleVertices[m_triangleVertexCount + i].position = pos;
	}

	unsigned short* indices = pCmd->getTriangles().indices;
	size_t indexCount = pCmd->getTriangles().indexCount;
	for (int i = 0; i < indexCount; i++) {
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

	m_pTriangleBatchToDraw[0].pCommand = nullptr;
	m_pTriangleBatchToDraw[0].indicesToDraw = 0;
	m_pTriangleBatchToDraw[0].offset = 0;

	m_triangleVertexCount = 0;
	m_triangleIndexCount = 0;
	int batchTotal = 0;
	bool firstCommand = true;

	for (const auto& cmd : m_trianglesCommands) {
		trianglesVerticesAndIndices(cmd, 0);

		if (!firstCommand) {
			batchTotal++;
			m_pTriangleBatchToDraw[batchTotal].offset = m_pTriangleBatchToDraw[batchTotal - 1].offset + m_pTriangleBatchToDraw[batchTotal - 1].indicesToDraw;
		}

		m_pTriangleBatchToDraw[batchTotal].pCommand = cmd;
		m_pTriangleBatchToDraw[batchTotal].indicesToDraw = cmd->getTriangles().indexCount;

		if (batchTotal + 1 >= m_triangleBatchToDrawSize) {
			m_triangleBatchToDrawSize *= 1.4;

			if (m_pTriangleBatchToDraw != nullptr) {
				m_pTriangleBatchToDraw = static_cast<TriangleBatchToDraw*>(std::realloc(m_pTriangleBatchToDraw, sizeof(TriangleBatchToDraw) * m_triangleBatchToDrawSize));
			}
		}

		firstCommand = false;
	}
	batchTotal++;

	m_pVertexBuffer->updateData(m_triangleVertices, sizeof(m_triangleVertices[0]) * m_triangleVertexCount);
	m_pIndexBuffer->updateData(m_triangleIndices, sizeof(m_triangleIndices[0]) * m_triangleIndexCount);

	for (int i = 0; i < batchTotal; i++) {
		auto& drawInfo = m_pTriangleBatchToDraw[i];
		
		size_t offset = sizeof(m_triangleIndices[0]) * drawInfo.offset;
		glDrawElements(GL_TRIANGLES, drawInfo.indicesToDraw, GL_UNSIGNED_SHORT, reinterpret_cast<void*>(offset));
	}

	m_trianglesCommands.clear();
}

void Renderer::drawCustomCommand(RenderCommand* command)
{
	CustomCommand* cmd = static_cast<CustomCommand*>(command);
	ProgramState& programState = cmd->getProgramState();

	Program* pProgram = programState.getProgram();
	pProgram->use();

	Scene* scene = Game::getInstance()->getCurrentScene();
	glm::mat4 projection = scene->getDefaultCamera()->getProjectionMatrix();
	glm::mat4 view = scene->getDefaultCamera()->getViewMatrix();

	glm::mat4 modelView = view * cmd->getModelView();

	pProgram->setUniform("uViewProj", projection);
	pProgram->setUniform("uWorldTransform", modelView);

	cmd->getVertexArray()->bind();

	const auto drawType = cmd->getDrawType();
	if (drawType == CustomCommand::DrawType::Element) {
		glDrawElements(OpenGLUtility::toGLPrimitive(cmd->getPrimitiveType()), 0, GL_UNSIGNED_SHORT, nullptr);
	} else {
		glDrawArrays(OpenGLUtility::toGLPrimitive(cmd->getPrimitiveType()), cmd->getVertexDrawStart(), cmd->getVertexDrawCount());
	}

	cmd->getVertexArray()->unbind();
}

OCF_END
