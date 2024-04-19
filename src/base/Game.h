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

	/** �Q�[���������� */
	bool init();

	/** �Q�[���̃��C�����[�v���J�n���� */
	void mainLoop();

	/** �Q�[���̃��C�����[�v���甲���� */
	void exit();

	glm::vec2 getVisibleSize() const;

	/**
	 * @brief �����_���[���擾����
	 * 
	 * @return �����_���[�̃|�C���^
	 */
	Renderer* getRenderer() const { return m_renderer; }

	/**
	 * @brief ���ݐݒ肳��Ă���V�[�����擾����
	 * 
	 * @return ���݂̃V�[���̃|�C���^
	 */
	Scene* getCurrentScene() const { return m_scene; }

	/**
	 * @brief �e�N�X�`���Ǘ��N���X���擾����
	 *
	 * @return �e�N�X�`���Ǘ��N���X�̃|�C���^
	 */
	TextureManager* getTextureManager() const { return m_textureManager; }

	/**
	 * @brief �t�H���g���擾����
	 *
	 * @return �t�H���g�̃|�C���^
	 */
	Font* getFont() { return m_font; }

	/**
	 * @brief �t���[�����[�g���擾����
	 *
	 * @return �t���[�����[�g
	 */
	float getFrameRate() const { return m_frameRate; }

protected:
	/** ���̓V�X�e�������� */
	void processInput();

	/** �Q�[���̍X�V�������s�� */
	void update();

	/** �Q�[���̕`�揈�����s�� */
	void draw();

	/** �f���^�^�C�����v�Z���� */
	void calculateDeltaTime();

	/** �f�o�b�O�p�̏��(FPS�Ȃ�)��\�� */
	void showStats();

private:
	friend Applicaiton;
	void onWindowSize(int width, int height);

private:
	/** �Q�[���N���X�̃C���X�^���X */
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
