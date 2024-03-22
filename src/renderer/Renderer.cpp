#include "Renderer.h"
#include "base/Game.h"
#include "renderer/OpenGLInclude.h"
#include "renderer/OpenGLUtility.h"
#include "renderer/CustomCommand.h"
#include <glm/gtc/type_ptr.hpp>

OCF_BEGIN

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
		printf("OpenGL info: %s\n", message);
		break;
	default:
		break;
	}
}
#endif

Renderer::Renderer()
	: m_viewport(0, 0, 0,0)
	, m_shaderManager(nullptr)
{
}

Renderer::~Renderer()
{
	destroy();
}

bool Renderer::init()
{
	// シェーダーを初期化
	m_shaderManager = ShaderManager::getInstance();

	// OpenGLのデバッグメッセージを有効にする
#ifdef _DEBUG
	//glEnable(GL_DEBUG_OUTPUT);
	//glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	//glDebugMessageCallback(debugMessageCallback, nullptr);
#endif

	return true;
}

void Renderer::destroy()
{
	// シェーダーの後始末
	m_shaderManager->removeAllPrograms();
	m_shaderManager->destroyInstance();
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
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (auto& sprite : m_sprites) {
		sprite->draw();
	}

	for (auto& lable : m_labels) {
		lable->draw();
	}

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	for (const auto& command : m_renderCommands) {

		const auto commandType = command->getType();

		switch (commandType) {
		case RenderCommand::Type::TrianglesCommand:
			break;
		case RenderCommand::Type::CustomCommand:
			drawCustomCommand(command);
			break;
		default:
			assert(false);
			break;
		}
	}

	m_renderCommands.clear();
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
