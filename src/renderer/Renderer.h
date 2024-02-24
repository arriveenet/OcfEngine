#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "2d/Sprite.h"
#include "2d/Label.h"

OCF_BEGIN

/**
 * @brief �����_���[�N���X
 */
class Renderer {
	std::vector<Sprite*> m_sprites;	//!< �X�v���C�g�z��
	std::vector<Label*> m_labels;
	glm::ivec4 m_viewport;

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

	void addLabel(Label* pLabel);

	/** �r���[�|�[�g��ݒ肷�� */
	void setViewPort(int x, int y, int width, int height);

	/** �r���[�|�[�g���擾���� */
	glm::ivec4 getViewport() const;

	/** �`�揈�����s�� */
	void draw();
};

OCF_END
