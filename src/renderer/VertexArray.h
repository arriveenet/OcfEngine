#pragma once
#include <string>
#include <unordered_map>
#include "OpenGLInclude.h"
#include "VertexBuffer.h"
#include "base/Macros.h"
#include "base/Types.h"

NS_OCF_BEGIN

class VertexArray {
public:
    struct Attribute {
        Attribute(std::string _name, int _index, int _size, bool _needToBeNormallized, std::size_t _offset)
            : name(_name), index(_index), size(_size), needToBeNormallized(_needToBeNormallized), offset(_offset)
        {
        }

        std::string name;
        int index = 0;
        int size = 3;
        bool needToBeNormallized = false;
        std::size_t offset = 0;
    };

    /** デフォルトコンストラクタ */
    VertexArray();
    /** デストラクタ */
    ~VertexArray();

    unsigned int getVertexCount() const { return m_vertexCount; }
    unsigned int getIndexCount() const { return m_indexCount; }

    GLuint getHandle() const { return m_vertexArray; }

    void bind();
    void unbind();

    std::size_t getStride() const { return m_stride; }
    void setStride(std::size_t stride);

    /**
     * @brief 頂点バッファを作成する
     * @param usage 予想される使用パターン
     */
    void createVertexBuffer(BufferUsage usage);
    /**
     * @brief インデックスバッファを作成する
     * @param usage 予想される使用パターン
     */
    void createIndexBuffer(BufferUsage usage);
 
    /**
     * @bfief 頂点バッファデータを作成し、初期化する
     * @param pData 初期化のためにコピーされるデータのポインタ
     * @param size データのサイズ(バイト単位)
     */
    void updateVertexBuffer(void* pData, size_t size);
    /**
     * @bfief インデックスバッファデータを作成し、初期化する
     * @param pData 初期化のためにコピーされるデータのポインタ
     * @param size データのサイズ(バイト単位)
     */
    void updateIndexBuffer(void* pData, size_t size);
    /**
     * @bfief 頂点バッファデータを更新する
     * @param pData コピーされるデータのポインタ
     * @param offset オフセット
     * @param size データのサイズ(バイト単位)
     */
    void updateVertexBuffer(void* pData, size_t offset, size_t size);
    /**
     * @bfief インデックスバッファデータを更新する
     * @param pData コピーされるデータのポインタ
     * @param offset オフセット
     * @param size データのサイズ(バイト単位)
     */
    void updateIndexBuffer(void* pData, size_t offset, size_t size);

    /**
     * @brief 設定した頂点属性データをバインドする
     */
    void bindVertexBuffer();
    /**
     * @brief 頂点属性データの配列を設定する
     * @param name 
     * @param index 頂点属性のインデックス
     * @param size 頂点属性のコンポーネント数。1,2,3,4である必要がある
     * @param needToBeNormallized データを正規化する場合true、直接指定する場合false
     * @param offset バッファデータ内のオフセット
     * @attention この関数をコールする前に頂点バッファを設定する必要がある。
     */
    void setAttribute(const std::string& name, int index, int size, bool needToBeNormallized, std::size_t offset);

    /**
     * @brief 頂点バッファが作成されているかどうかを返す
     * @retval true 頂点バッファが作成されている
     * @retval false 頂点バッファが作成されていない
     */
    bool isVertexBufferCreated() const { return m_isVertexBufferCreated; }

    /**
     * @brief インデックスバッファが作成されているかどうかを返す
     * @retval true インデックスバッファが作成されている
     * @retval false インデックスバッファが作成されていない
     */
    bool isIndexBufferCreated() const { return m_isIndexBufferCreated; }

private:
    GLuint m_vertexArray;

    VertexBuffer* m_vertexBuffer;
    VertexBuffer* m_indexBuffer;

    unsigned int m_vertexCount;
    unsigned int m_indexCount;

    bool m_isVertexBufferCreated;
    bool m_isIndexBufferCreated;

    std::size_t m_stride;
    std::unordered_map<std::string, Attribute> m_attributes;
};

NS_OCF_END
