#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "renderer/VertexBuffer.h"
#include "renderer/ShaderManager.h"
#include "renderer/RenderCommand.h"
#include "renderer/TrianglesCommand.h"
#include "renderer/RenderQueue.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

NS_OCF_BEGIN

/**
 * @brief �����_���[�N���X
 */
class Renderer {
public:
	static constexpr int VBO_SIZE = 0x10000;
	static constexpr int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;

	/** �R���X�g���N�^�[ */
	Renderer();
	/** �f�X�g���N�^�[ */
	~Renderer();

	/** �����_���[�̏��������s�� */
	bool init();
	/** �����_���[��j������ */
	void destroy();

	/** �t���[���̕`����n�߂� */
	void beginFrame();
	/** �t���[���̕`����I��� */
	void endFrame();

	/** �r���[�|�[�g��ݒ肷�� */
	void setViewPort(int x, int y, int width, int height);

	/** �r���[�|�[�g���擾���� */
	glm::ivec4 getViewport() const;

	/** �����_�[�R�}���h��ǉ����� */
	void addCommand(RenderCommand* command);

	/** �o�b�t�@���N���A���� */
	void clear();

	void clean();

	/** �`�揈�����s�� */
	void draw();

	uint32_t getDrawCallCount() const { return m_drawCallCount; }
	uint32_t getDrawVertexCount() const { return m_drawVertexCount; }

protected:
	void flush();
	void visitRenderQueue(RenderQueue& queue);
	void doVisitRenderQueue(const std::vector<RenderCommand*>& renderCommands);
	void processRenderCommand(RenderCommand* command);
	void trianglesVerticesAndIndices(TrianglesCommand* pCmd, unsigned int vertexBufferOffset);
	void drawTrianglesCommand();
	void drawCustomCommand(RenderCommand* command);

private:
	glm::ivec4 m_viewport;
	std::vector<TrianglesCommand*> m_trianglesCommands;
	std::vector<RenderQueue> m_renderGroups;

	uint32_t m_drawCallCount;
	uint32_t m_drawVertexCount;

	struct TriangleBatchToDraw {
		TrianglesCommand* pCommand = nullptr;
		unsigned int indicesToDraw = 0;
		unsigned int offset = 0;
	};
	TriangleBatchToDraw* m_pTriangleBatchToDraw;
	int m_triangleBatchToDrawSize;

	Vertex3fC3fT2f m_triangleVertices[VBO_SIZE];
	unsigned short m_triangleIndices[INDEX_VBO_SIZE];
	unsigned int m_triangleVertexCount;
	unsigned int m_triangleIndexCount;
	VertexArray* m_pVertexArray;
};

NS_OCF_END
