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
 * @brief レンダラークラス
 */
class Renderer {
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

	/** ラベルを追加する */
	void addLabel(Label* pLabel);

	/** ビューポートを設定する */
	void setViewPort(int x, int y, int width, int height);

	/** ビューポートを取得する */
	glm::ivec4 getViewport() const;

	/** レンダーコマンドを追加する */
	void addCommand(RenderCommand* command);

	/** 描画処理を行う */
	void draw();

private:
	std::vector<Sprite*> m_sprites;	//!< スプライト配列
	std::vector<Label*> m_labels;
	glm::ivec4 m_viewport;
	ShaderManager* m_shaderManager;
	std::vector<RenderCommand*> m_renderCommands;
};

OCF_END
