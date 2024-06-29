#pragma once
#include "base/Macros.h"
#include "base/Config.h"

NS_OCF_BEGIN

/**
 * @brief �Q�[���I�u�W�F�N�g
 * 
 * �Q�ƃJ�E���g�ɂ�胁�������Ǘ�����
 */
class GameObject {
public:
	/** �R���X�g���N�^ */
	GameObject();
	/** �f�X�g���N�^ */
	virtual ~GameObject();

	/**
	 * @brief �ŗL��ID���擾����
	 * @return ID
	 */
	unsigned int getID() const;

	/**
	 * @brief �Q�ƃJ�E���g���C���N�������g����
	 */
	void retain();

	/**
	 * @brief �Q�ƃJ�E���g���f�N�������g����
	 * �����A�f�N�������g���Q�ƃJ�E���g��0�̏ꍇ�A�Q�[���I�u�W�F�N�g���������
	 */
	void release();

	/**
	 * @brief ����������s��
	 */
	GameObject* autorelease();

	/**
	 * @brief �Q�ƃJ�E���g���擾����
	 */
	unsigned int getReferenceCount();

protected:
	unsigned int m_referenceCount;	//!< �Q�ƃJ�E���g

private:
	unsigned int m_id;	//!< ID
};

NS_OCF_END
