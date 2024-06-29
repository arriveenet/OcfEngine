#pragma once
#include "base/Macros.h"
#include "base/Config.h"

NS_OCF_BEGIN

/**
 * @brief ゲームオブジェクト
 * 
 * 参照カウントによりメモリを管理する
 */
class GameObject {
public:
	/** コンストラクタ */
	GameObject();
	/** デストラクタ */
	virtual ~GameObject();

	/**
	 * @brief 固有のIDを取得する
	 * @return ID
	 */
	unsigned int getID() const;

	/**
	 * @brief 参照カウントをインクリメントする
	 */
	void retain();

	/**
	 * @brief 参照カウントをデクリメントする
	 * もし、デクリメントし参照カウントが0の場合、ゲームオブジェクトを解放する
	 */
	void release();

	/**
	 * @brief 自動解放を行う
	 */
	GameObject* autorelease();

	/**
	 * @brief 参照カウントを取得する
	 */
	unsigned int getReferenceCount();

protected:
	unsigned int m_referenceCount;	//!< 参照カウント

private:
	unsigned int m_id;	//!< ID
};

NS_OCF_END
