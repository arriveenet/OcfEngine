#pragma once
#include <vector>
#include <glm/glm.hpp>
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

private:
	std::vector<Sprite*> m_sprites;	//!< �X�v���C�g�z��
	std::vector<Label*> m_labels;
	glm::ivec4 m_viewport;
	ShaderManager* m_shaderManager;
	std::vector<RenderCommand*> m_renderCommands;
};

OCF_END
