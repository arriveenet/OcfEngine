#pragma once
#include <vector>
#include "2d/Sprite.h"

/**
 * @brief �����_���[�N���X
 */
class Renderer {
	std::vector<Sprite*> m_sprites;	//!< �X�v���C�g�z��

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

	/** �`�揈�����s�� */
	void draw();
};