#pragma once
#include "GameObject.h"
#include "Scene/Scene.h"
#include "renderer/Renderer.h"

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

	void onKeyEnvet(int key, int scancode, int action, int mods);

	glm::vec2 getVisibleSize();

private:
	void update();
	void draw();

public:
	Renderer* getRenderer() const { return m_pRenderer; }
	Scene* getCurrentScene() const { return m_pScene; }

private:
	static Game* s_sharedGame;
	bool m_running;
	Renderer* m_pRenderer;
	Scene* m_pScene;
};