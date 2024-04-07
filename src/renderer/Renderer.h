#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "VertexBuffer.h"
#include "ShaderManager.h"
#include "RenderCommand.h"
#include "TrianglesCommand.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

OCF_BEGIN

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

	/**
	 * @brief �X�v���C�g��ǉ�����
	 * 
	 * @param pSprite	�ǉ�����X�v���C�g
	 */
	void addSprite(Sprite* pSprite);

	/**
	 * @brief �X�v���C�g���폜����
	 *
	 * @param pSprite	�폜����X�v���C�g
	 */
	void removeSprite(Sprite* pSprite);

	/** ���x����ǉ����� */
	void addLabel(Label* pLabel);

	/** �r���[�|�[�g��ݒ肷�� */
	void setViewPort(int x, int y, int width, int height);

	/** �r���[�|�[�g���擾���� */
	glm::ivec4 getViewport() const;

	/** �����_�[�R�}���h��ǉ����� */
	void addCommand(RenderCommand* command);

	/** �`�揈�����s�� */
	void draw();

protected:
	void flush();
	void trianglesVerticesAndIndices(TrianglesCommand* pCmd, unsigned int vertexBufferOffset);
	void drawTrianglesCommand();
	void drawCustomCommand(RenderCommand* command);

private:
	std::vector<Sprite*> m_sprites;
	std::vector<Label*> m_labels;
	glm::ivec4 m_viewport;
	ShaderManager* m_shaderManager;
	std::vector<RenderCommand*> m_renderCommands;
	std::vector<TrianglesCommand*> m_trianglesCommands;

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

OCF_END
