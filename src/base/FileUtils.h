#pragma once
#include <string>
#include "Macros.h"

OCF_BEGIN

/**
 * @brief �t�@�C�����[�e�B���e�B�N���X
 */
class FileUtils {
public:
	/** FileUtils�̃C���X�^���X���擾���� */
	static FileUtils* getInstance();

	/** FileUtils�̃C���X�^���X��j������ */
	static void destroyInstance();

	/** �f�X�g���N�^�[ */
	~FileUtils();

	/**
	 * @brief �t�@�C���������΃p�X���擾����
	 * @param filename �t�@�C����
	 * @return �ϊ����ꂽ��΃p�X
	 */
	std::string getFullPath(const std::string& filename) const;

	/**
	 * @brief �t�@�C��������e�̐�΃p�X���擾����
	 * @param filename �t�@�C����
	 * @return �ϊ����ꂽ�e�̐�΃p�X
	 */
	std::string getParentFullPath(const std::string& filename) const;

	/**
	 * @brief assets�t�H���_�̃p�X���擾��
	 * @return assets�t�H���_�̃p�X
	 */
	std::string getAssetsPath() const;

protected:
	/** �f�t�H���g�R���X�g���N�^�[ */
	FileUtils();

private:
	/** FileUtils�̃V���O���g���|�C���^�[ */
	static FileUtils* s_sharedFileUtils;
};

OCF_END