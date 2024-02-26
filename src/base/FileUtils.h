#pragma once
#include <string>
#include "Macros.h"

OCF_BEGIN

/**
 * @brief ファイルユーティリティクラス
 */
class FileUtils {
public:
	/** FileUtilsのインスタンスを取得する */
	static FileUtils* getInstance();

	/** FileUtilsのインスタンスを破棄する */
	static void destroyInstance();

	/** デストラクター */
	~FileUtils();

	/**
	 * @brief ファイル名から絶対パスを取得する
	 * @param filename ファイル名
	 * @return 変換された絶対パス
	 */
	std::string getFullPath(const std::string& filename) const;

	/**
	 * @brief ファイル名から親の絶対パスを取得する
	 * @param filename ファイル名
	 * @return 変換された親の絶対パス
	 */
	std::string getParentFullPath(const std::string& filename) const;

	/**
	 * @brief assetsフォルダのパスを取得す
	 * @return assetsフォルダのパス
	 */
	std::string getAssetsPath() const;

protected:
	/** デフォルトコンストラクター */
	FileUtils();

private:
	/** FileUtilsのシングルトンポインター */
	static FileUtils* s_sharedFileUtils;
};

OCF_END