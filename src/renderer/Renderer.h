#pragma once
#include <vector>
#include "2d/Sprite.h"

/**
 * @brief レンダラークラス
 */
class Renderer {
	std::vector<Sprite*> m_sprites;	//!< スプライト配列

public:
	/** コンストラクター */
	Renderer();
	/** デストラクター */
	~Renderer();

	/** レンダラーの初期化を行う */
	bool init();
	/** レンダラーを破棄する */
	void destroy();

	/**
	 * @brief スプライトを追加する
	 * 
	 * @param pSprite	追加するスプライト
	 */
	void addSprite(Sprite* pSprite);

	/**
	 * @brief スプライトを削除する
	 *
	 * @param pSprite	削除するスプライト
	 */
	void removeSprite(Sprite* pSprite);

	/** 描画処理を行う */
	void draw();
};