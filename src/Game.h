#pragma once
#include "GameObject.h"
#include "Scene/Scene.h"
#include "renderer/Renderer.h"
#include "base/Font.h"
#include "base/Input.h"

/**
 * @brief �Q�[���N���X
 * �Q�[���̏�ԊǗ��Ȃǂ��s���V���O���g���N���X�B
 */
class Game : public GameObject {
public:
	Game();
	~Game();

	/**
	 * @brief �Q�[���̃C���X�^���X���擾����
	 *
	 * @return �Q�[���N���X�̃C���X�^���X
	 */
	static Game* getInstance();

	bool init();

	void mainLoop();
	void exit();

	glm::vec2 getVisibleSize();

private:
	void processInput();
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
	Font m_font;
	Input m_input;
};