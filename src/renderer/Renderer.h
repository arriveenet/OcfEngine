#pragma once
#include <vector>
#include "2d/Sprite.h"
#include "2d/Label.h"

OCF_BEGIN

/**
 * @brief �����_���[�N���X
 */
class Renderer {
	std::vector<Sprite*> m_sprites;	//!< �X�v���C�g�z��
	std::vector<Label*> m_labels;

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

	/** �`�揈�����s�� */
	void draw();
};

OCF_END
