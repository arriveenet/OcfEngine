#pragma once
#include <chrono>
#include <stack>
#include <vector>
#include "base/Types.h"
#include "base/GameObject.h"

namespace ocf {

class Scene;
class Label;
class Input;
class Renderer;
class GLView;
class TextureManager;
class EventDispatcher;

/**
 * @brief ゲームクラス
 * ゲームの状態管理などを行うシングルトンクラス。
 */
class Game : public GameObject {
public:
    enum class Projection {
        _2D,
        _3D
    };

    /** コンストラクタ */
    Game();
    /** デストラクタ */
    ~Game();

    /**
     * @brief ゲームのインスタンスを取得する
     * @return ゲームクラスのインスタンス
     */
    static Game* getInstance();

    /**
     * @brief ゲームのインスタンスを破棄する
     */
    static void destroyInstance();

    /** ゲームを初期化 */
    bool init();

    /** ゲームのメインループを開始する */
    void mainLoop();

    /** ゲームのメインループから抜ける */
    void exit();

    /** ゲームの終了処理 */
    void cleanup();

    /** 起動時のシーンを設定する */
    void runWithScene(Scene* pScene);

    /** シーンを入れ替える */
    void replaceScene(Scene* pScene);

    /** シーンスタックにプッシュする */
    void pushScene(Scene* pScene);

    /** シーンスタックをポップする */
    void popScene();

    /** 次のシーンを設定する */
    void setNextScene();

    /**
     * @brief 表示サイズを取得する
     * @return 表示サイズ
     */
    glm::vec2 getVisibleSize() const;

    /**
     * @brief ウィンドウサイズを取得する
     * @return ウィンドウサイズ
     */
    const glm::vec2& getResolutionSize() const;

    /**
     * @brief カメラと近くのクリッピング フレーム間の距離を取得
     * @return フレーム間の距離
     */
    float getZEye() const;

    /**
     * @brief 投影方法を設定する
     * @param[in] 投影方法
     */
    void setProjection(Projection projection);

    /**
     * @brief 投影方法を取得する
     * @return 投影方法
     */
    Projection getProjection() const { return m_projection; }

    /**
     * @brief ビューポートを設定する
     */
    void setViewport();

    /**
     * @brief レンダラーを取得する
     * @return レンダラーのポインタ
     */
    Renderer* getRenderer() const { return m_renderer; }

    /**
     * @brief 現在設定されているシーンを取得する
     * @return 現在のシーンのポインタ
     */
    Scene* getCurrentScene() const { return m_currentScene; }

    /**
     * @brief OpenGLのビューを取得
     * @return OpenGLのビュー
     */
    GLView* getGLView() { return m_glView; }

    /**
     * @brief OpenGLのビューを設定
     * @param[in] OpenGLのビュー
     */
    void setGLView(GLView* glView);

    /**
     * @brief テクスチャ管理クラスを取得する
     * @return テクスチャ管理クラスのポインタ
     */
    TextureManager* getTextureManager() const { return m_textureManager; }

    /**
     * @brief イベントディスパッチャを取得する
     * @return イベントディスパッチャのポインタ
     */
    EventDispatcher* getEventDispatcher() const { return m_eventDispatcher; }

    /**
     * @brief フレームレートを取得する
     * @return フレームレート
     */
    float getFrameRate() const { return m_frameRate; }

    /** @brief 行列スタックを初期化 */
    void initMatrixStack();

    /**
     * @brief 行列スタックを単位行列に置き換える
     * @param[in] type 行列スタックの種類
     */
    void loadIdentityMatrix(MatrixStack type);

    /**
     * @brief 行列スタックを指定された行列に置き換える
     * @param[in] type 行列スタックの種類
     * @param[in] matrix 置き換える行列
     */
    void loadMatrix(MatrixStack type, const glm::mat4& matrix);

    /**
     * @brief 行列スタックを指定された行列で乗算し置き換える
     * @param[in] type 行列スタックの種類
     * @param[in] matrix 乗算する行列
     */
    void multiplyMatrix(MatrixStack type, const glm::mat4& matrix);

    /**
     * @brief 行列スタックをプッシュする
     * @param[in] type 行列スタックの種別
     */
    void pushMatrix(MatrixStack type);

    /**
     * @brief 行列スタックをポップする
     * @param[in] type 行列スタックの種別
     */
    void popMatrix(MatrixStack type);

    /**
     * @brief 行列を取得する
     * @param[in] type 行列スタックの種別
     */
    const glm::mat4& getMatrix(MatrixStack type);

protected:
    /** ゲームの更新処理を行う */
    void update();

    /** ゲームの描画処理を行う */
    void draw();

    /** デルタタイムを計算する */
    void calculateDeltaTime();

    /** デバッグ用の状態(FPSなど)を表示 */
    void showStats();

    void createStatsLabel();

private:
    /** ゲームクラスのインスタンス */
    static Game* s_sharedGame;
    bool m_running;
    bool m_cleanupInNextLoop;

    float m_deltaTime;
    std::chrono::steady_clock::time_point m_lastUpdate;

    Projection m_projection;

    unsigned int m_frames = 0;
    float m_accumulator = 0.0f;
    float m_frameRate = 0.0f;

    glm::vec2 m_windowSize;
    glm::vec2 m_resolutionSize;

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

    friend class GLView;
};

} // namespace ocf
