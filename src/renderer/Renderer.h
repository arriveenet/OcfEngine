#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "VertexBuffer.h"
#include "ShaderManager.h"
#include "RenderCommand.h"
#include "TrianglesCommand.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

OCF_BEGIN

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

	/** ビューポートを設定する */
	void setViewPort(int x, int y, int width, int height);

	/** ビューポートを取得する */
	glm::ivec4 getViewport() const;

	/** レンダーコマンドを追加する */
	void addCommand(RenderCommand* command);

	/** 描画処理を行う */
	void draw();

protected:
	void flush();
	void trianglesVerticesAndIndices(TrianglesCommand* pCmd, unsigned int vertexBufferOffset);
	void drawTrianglesCommand();
	void drawCustomCommand(RenderCommand* command);

private:
	glm::ivec4 m_viewport;
	ShaderManager* m_shaderManager;
	std::vector<RenderCommand*> m_renderCommands;
	std::vector<TrianglesCommand*> m_trianglesCommands;

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

OCF_END
