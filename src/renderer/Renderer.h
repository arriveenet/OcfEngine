#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "renderer/VertexBuffer.h"
#include "renderer/ShaderManager.h"
#include "renderer/RenderCommand.h"
#include "renderer/TrianglesCommand.h"
#include "renderer/RenderQueue.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

NS_OCF_BEGIN

/**
 * @brief レンダラークラス
 */
class Renderer {
public:
    static constexpr int VBO_SIZE = 0x10000;
    static constexpr int INDEX_VBO_SIZE = VBO_SIZE * 6 / 4;

    /** コンストラクター */
    Renderer();
    /** デストラクター */
    ~Renderer();

    /** レンダラーの初期化を行う */
    bool init();
    /** レンダラーを破棄する */
    void destroy();

    /** フレームの描画を始める */
    void beginFrame();
    /** フレームの描画を終わる */
    void endFrame();

    /** ビューポートを設定する */
    void setViewPort(int x, int y, int width, int height);

    /** ビューポートを取得する */
    glm::ivec4 getViewport() const;

    /** ポイントサイズを設定する */
    void setPointSize(float size) const;

    /** ポイントサイズを取得する */
    float getPointSize() const;

    /** ライン幅を設定する */
    void setLineWidth(float size) const;

    /** ライン幅を取得する */
    float getLineWidth() const;

    /** レンダーコマンドを追加する */
    void addCommand(RenderCommand* command);

    /** バッファをクリアする */
    void clear();

    /** バッファをクリアする */
    void clean();

    /** 描画処理を行う */
    void draw();

    uint32_t getDrawCallCount() const { return m_drawCallCount; }
    uint32_t getDrawVertexCount() const { return m_drawVertexCount; }

protected:
    void flush();
    void flush2D();
    void flush3D();
    void visitRenderQueue(RenderQueue& queue);
    void doVisitRenderQueue(const std::vector<RenderCommand*>& renderCommands);
    void processRenderCommand(RenderCommand* command);
    void trianglesVerticesAndIndices(TrianglesCommand* pCmd, unsigned int vertexBufferOffset);
    void drawTrianglesCommand();
    void drawCustomCommand(RenderCommand* command);
    void drawMeshCommand(RenderCommand* command);

private:
    glm::ivec4 m_viewport;
    std::vector<TrianglesCommand*> m_trianglesCommands;
    std::vector<RenderQueue> m_renderGroups;

    uint32_t m_drawCallCount;
    uint32_t m_drawVertexCount;

    struct TriangleBatchToDraw {
        TrianglesCommand* pCommand = nullptr;
        unsigned int indicesToDraw = 0;
        unsigned int offset = 0;
    };
    TriangleBatchToDraw* m_pTriangleBatchToDraw;
    int m_triangleBatchToDrawSize;

    Vertex3fC3fT2f m_triangleVertices[VBO_SIZE];
    unsigned short m_triangleIndices[INDEX_VBO_SIZE];
    unsigned int m_triangleVertexCount;
    unsigned int m_triangleIndexCount;
    VertexArray* m_pVertexArray;
};

NS_OCF_END
