#pragma once
#include <chrono>
#include <stack>
#include <vector>
#include "base/types.h"
#include "base/GameObject.h"

NS_OCF_BEGIN

class Scene;
class Label;
class Input;
class Renderer;
class GLView;
class TextureManager;
class EventDispatcher;

/**
 * @brief �Q�[���N���X
 * �Q�[���̏�ԊǗ��Ȃǂ��s���V���O���g���N���X�B
 */
class Game : public GameObject {
public:
    /** �R���X�g���N�^ */
    Game();
    /** �f�X�g���N�^ */
    ~Game();

    /**
     * @brief �Q�[���̃C���X�^���X���擾����
     * @return �Q�[���N���X�̃C���X�^���X
     */
    static Game* getInstance();

    /**
     * @brief �Q�[���̃C���X�^���X��j������
     */
    static void destroyInstance();

    /** �Q�[���������� */
    bool init();

    /** �Q�[���̃��C�����[�v���J�n���� */
    void mainLoop();

    /** �Q�[���̃��C�����[�v���甲���� */
    void exit();

    /** �N�����̃V�[����ݒ肷�� */
    void runWithScene(Scene* pScene);

    /** �V�[�������ւ��� */
    void replaceScene(Scene* pScene);

    /** �V�[���X�^�b�N�Ƀv�b�V������ */
    void pushScene(Scene* pScene);

    /** �V�[���X�^�b�N���|�b�v���� */
    void popScene();

    /** ���̃V�[����ݒ肷�� */
    void setNextScene();

    /**
     * @brief �\���T�C�Y���擾����
     * @return �\���T�C�Y
     */
    glm::vec2 getVisibleSize() const;

    /**
     * @brief �J�����Ƌ߂��̃N���b�s���O �t���[���Ԃ̋������擾
     * @return �t���[���Ԃ̋���
     */
    float getZEye() const;

    /**
     * @brief �����_���[���擾����
     * @return �����_���[�̃|�C���^
     */
    Renderer* getRenderer() const { return m_renderer; }

    /**
     * @brief ���ݐݒ肳��Ă���V�[�����擾����
     * @return ���݂̃V�[���̃|�C���^
     */
    Scene* getCurrentScene() const { return m_currentScene; }

    /**
     * @brief OpenGL�̃r���[���擾
     * @return OpenGL�̃r���[
     */
    GLView* getGLView() { return m_glView; }

    /**
     * @brief OpenGL�̃r���[��ݒ�
     * @param[in] OpenGL�̃r���[
     */
    void setGLView(GLView* glView);

    /**
     * @brief �e�N�X�`���Ǘ��N���X���擾����
     * @return �e�N�X�`���Ǘ��N���X�̃|�C���^
     */
    TextureManager* getTextureManager() const { return m_textureManager; }

    /**
     * @brief �C�x���g�f�B�X�p�b�`�����擾����
     * @return �C�x���g�f�B�X�p�b�`���̃|�C���^
     */
    EventDispatcher* getEventDispatcher() const { return m_eventDispatcher; }

    /**
     * @brief �t���[�����[�g���擾����
     * @return �t���[�����[�g
     */
    float getFrameRate() const { return m_frameRate; }

    /** @brief �s��X�^�b�N�������� */
    void initMatrixStack();

    /**
     * @brief �s��X�^�b�N��P�ʍs��ɒu��������
     * @param[in] type �s��X�^�b�N�̎��
     */
    void loadIdentityMatrix(MatrixStack type);

    /**
     * @brief �s��X�^�b�N���w�肳�ꂽ�s��ɒu��������
     * @param[in] type �s��X�^�b�N�̎��
     * @param[in] matrix �u��������s��
     */
    void loadMatrix(MatrixStack type, const glm::mat4& matrix);

    /**
     * @brief �s��X�^�b�N���w�肳�ꂽ�s��ŏ�Z���u��������
     * @param[in] type �s��X�^�b�N�̎��
     * @param[in] matrix ��Z����s��
     */
    void multiplyMatrix(MatrixStack type, const glm::mat4& matrix);

    /**
     * @brief �s��X�^�b�N���v�b�V������
     * @param[in] type �s��X�^�b�N�̎��
     */
    void pushMatrix(MatrixStack type);

    /**
     * @brief �s��X�^�b�N���|�b�v����
     * @param[in] type �s��X�^�b�N�̎��
     */
    void popMatrix(MatrixStack type);

    /**
     * @brief �s����擾����
     * @param[in] type �s��X�^�b�N�̎��
     */
    const glm::mat4& getMatrix(MatrixStack type);

protected:
    /** �Q�[���̍X�V�������s�� */
    void update();

    /** �Q�[���̕`�揈�����s�� */
    void draw();

    /** �f���^�^�C�����v�Z���� */
    void calculateDeltaTime();

    /** �f�o�b�O�p�̏��(FPS�Ȃ�)��\�� */
    void showStats();

    void createStatsLabel();

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
    Scene* m_currentScene;
    Scene* m_nextScene;
    std::vector<Scene*> m_sceneStack;
    GLView* m_glView;
    TextureManager* m_textureManager;

    EventDispatcher* m_eventDispatcher;

    Label* m_pFPSLabel = nullptr;
    Label* m_pDrawCallLabel = nullptr;
    Label* m_pDrawVertexLabel = nullptr;

    std::stack<glm::mat4> m_projectionMatrixStack;
    std::stack<glm::mat4> m_modelViewMatrixStack;
};

NS_OCF_END
