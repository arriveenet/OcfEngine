#pragma once
#include "Scene/Scene.h"
#include "renderer/Renderer.h"

/**
 * @brief ゲームクラス
 * ゲームの状態管理などを行うシングルトンクラス。
 */
class Game {
	Game();

public:
	virtual ~Game();

	/**
	 * @brief ゲームのインスタンスを取得する
	 * @return ゲームクラスのインスタンス
	 */
	static Game* getInstance();
	bool init();
	void destroy();

	void mainLoop();
	void exit();

	glm::vec2 getVisibleSize();

private:
	void update();
	void draw();

public:
	Renderer* getRenderer() const { return m_pRenderer; }
	Scene* getCurrentScene() const { return m_pScene; }

private:
	bool m_running;
	Renderer* m_pRenderer;
	Scene* m_pScene;
};