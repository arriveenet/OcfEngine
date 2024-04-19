#pragma once
#include <chrono>
#include "platform/Application.h"
#include "Scene/Scene.h"
#include "base/GameObject.h"
#include "base/Font.h"
#include "input/Input.h"
#include "2d/Label.h"
#include "renderer/Renderer.h"
#include "renderer/TextureManager.h"

OCF_BEGIN

/**
 * @brief ゲームクラス
 * ゲームの状態管理などを行うシングルトンクラス。
 */
class Game : public GameObject {
public:
	Game();
	~Game();

	/**
	 * @brief ゲームのインスタンスを取得する
	 *
	 * @return ゲームクラスのインスタンス
	 */
	static Game* getInstance();

	/** ゲームを初期化 */
	bool init();

	/** ゲームのメインループを開始する */
	void mainLoop();

	/** ゲームのメインループから抜ける */
	void exit();

	glm::vec2 getVisibleSize() const;

	/**
	 * @brief レンダラーを取得する
	 * 
	 * @return レンダラーのポインタ
	 */
	Renderer* getRenderer() const { return m_renderer; }

	/**
	 * @brief 現在設定されているシーンを取得する
	 * 
	 * @return 現在のシーンのポインタ
	 */
	Scene* getCurrentScene() const { return m_scene; }

	/**
	 * @brief テクスチャ管理クラスを取得する
	 *
	 * @return テクスチャ管理クラスのポインタ
	 */
	TextureManager* getTextureManager() const { return m_textureManager; }

	/**
	 * @brief フォントを取得する
	 *
	 * @return フォントのポインタ
	 */
	Font* getFont() { return m_font; }

	/**
	 * @brief フレームレートを取得する
	 *
	 * @return フレームレート
	 */
	float getFrameRate() const { return m_frameRate; }

protected:
	/** 入力システムを処理 */
	void processInput();

	/** ゲームの更新処理を行う */
	void update();

	/** ゲームの描画処理を行う */
	void draw();

	/** デルタタイムを計算する */
	void calculateDeltaTime();

	/** デバッグ用の状態(FPSなど)を表示 */
	void showStats();

private:
	friend Applicaiton;
	void onWindowSize(int width, int height);

private:
	/** ゲームクラスのインスタンス */
	static Game* s_sharedGame;
	bool m_running;

	float m_deltaTime;
	std::chrono::steady_clock::time_point m_lastUpdate;

	unsigned int m_frames = 0;
	float m_accumulator = 0.0f;
	float m_frameRate = 0.0f;

	Renderer* m_renderer;
	Scene* m_scene;
	TextureManager* m_textureManager;

	Font* m_font;
	Input* m_input;

	Label* m_pFPSLabel = nullptr;
	Label* m_pDrawCallLabel = nullptr;
	Label* m_pDrawVertexLabel = nullptr;
};

OCF_END
