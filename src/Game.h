#pragma once
#include "Scene/Scene.h"
#include "renderer/Renderer.h"

/**
 * @brief �Q�[���N���X
 * �Q�[���̏�ԊǗ��Ȃǂ��s���V���O���g���N���X�B
 */
class Game {
	Game();

public:
	virtual ~Game();

	/**
	 * @brief �Q�[���̃C���X�^���X���擾����
	 * @return �Q�[���N���X�̃C���X�^���X
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