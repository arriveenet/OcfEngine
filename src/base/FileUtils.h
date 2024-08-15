#pragma once
#include <string>
#include <vector>
#include "Macros.h"

NS_OCF_BEGIN

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
	 * @brief �T�[�`�p�X�Ƀp�X��ǉ�����
	 * @param path �ǉ�����p�X
	 * @param front ��ԑO�ɒǉ����邩�ǂ���
	 */
	void addSearchPath(const std::string& path, bool front = false);

	/**
	 * @brief �T�[�`�p�X���擾����
	 * @return �T�[�`�p�X
	 */
	const std::vector <std::string>& getSearchPath() const;

	/**
	 * @brief assets�t�H���_�̃p�X���擾����
	 * @return assets�t�H���_�̃p�X
	 */
	std::string getAssetsPath() const;

	/**
	 * @brief �t�@�C��������t���p�X���擾����
	 * @param �t�@�C����
	 * @return �t���p�X�B���݂��Ȃ������ꍇ�󕶎���ԋp����
	 */
	std::string fullPathForFilename(const std::string& filename) const;

	/**
	 * @brief �t�@�C�����̕�������擾����
	 * @param �t�@�C����
	 * @return �t�@�C���̃R���e���c
	 */
	std::string getStringFromFile(std::string_view filename) const;

	/**
	 * @brief �t�@�C�������݂��邩�`�F�b�N����
	 * @param �t�@�C����
	 * @return �t�@�C�������݂���ꍇtrue�A���݂��Ȃ��ꍇfalse��ԋp
	 */
	bool isFileExist(const std::string& filename) const;

protected:
	/** �f�t�H���g�R���X�g���N�^�[ */
	FileUtils();

	/**
	 * @brief FileUtils�̃C���X�^���X������������
	 */
	bool init();

	/**
	 * @brief �t�@�C��������p�X���擾
	 * @param filename �t�@�C����
	 * @param searchPath �����p�X
	 * @return �t�@�C�������݂����ꍇ���̃p�X��ԋp�A���݂��Ȃ������ꍇ�󕶎���ԋp����
	 */
	std::string getPathForFilename(const std::string& filename, const std::string& searchPath) const;

private:
	/** FileUtils�̃V���O���g���|�C���^�[ */
	static FileUtils* s_sharedFileUtils;
	
	/** exe�̂���f�B���N�g�� */
	static std::string s_exeDirectory;

	/** �t�@�C������������p�X�̔z�� */
	std::vector<std::string> m_searchPathArray;
	/** �f�t�H���g��assets�̃��[�g�p�X */
	std::string m_defaultAssetsRootPath;
};

NS_OCF_END