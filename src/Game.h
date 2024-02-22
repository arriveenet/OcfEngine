#pragma once
#include <chrono>
#include "GameObject.h"
#include "Scene/Scene.h"
#include "base/Font.h"
#include "base/Input.h"
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

	bool init();

	void mainLoop();
	void exit();

	glm::vec2 getVisibleSize();

protected:
	void processInput();
	void update();
	void draw();

	void calculateDeltaTime();

public:
	Renderer* getRenderer() const { return m_pRenderer; }
	Scene* getCurrentScene() const { return m_pScene; }
	TextureManager* getTextureManager() const { return m_pTextureManager; }
	Font* getFont() { return &m_font; }

private:
	static Game* s_sharedGame;
	bool m_running;

	float m_deltaTime;
	std::chrono::steady_clock::time_point m_lastUpdate;

	unsigned int m_frames = 0;
	float m_accumulator = 0.0f;

	Renderer* m_pRenderer;
	Scene* m_pScene;
	TextureManager* m_pTextureManager;

	Font m_font;
	Input m_input;
};

OCF_END
