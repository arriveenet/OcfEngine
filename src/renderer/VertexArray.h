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

    /** �f�t�H���g�R���X�g���N�^ */
    VertexArray();
    /** �f�X�g���N�^ */
    ~VertexArray();

    unsigned int getVertexCount() const { return m_vertexCount; }
    unsigned int getIndexCount() const { return m_indexCount; }

    GLuint getHandle() const { return m_vertexArray; }

    void bind();
    void unbind();

    std::size_t getStride() const { return m_stride; }
    void setStride(std::size_t stride);

    /**
     * @brief ���_�o�b�t�@���쐬����
     * @param usage �\�z�����g�p�p�^�[��
     */
    void createVertexBuffer(BufferUsage usage);
    /**
     * @brief �C���f�b�N�X�o�b�t�@���쐬����
     * @param usage �\�z�����g�p�p�^�[��
     */
    void createIndexBuffer(BufferUsage usage);
 
    /**
     * @bfief ���_�o�b�t�@�f�[�^���쐬���A����������
     * @param pData �������̂��߂ɃR�s�[�����f�[�^�̃|�C���^
     * @param size �f�[�^�̃T�C�Y(�o�C�g�P��)
     */
    void updateVertexBuffer(void* pData, size_t size);
    /**
     * @bfief �C���f�b�N�X�o�b�t�@�f�[�^���쐬���A����������
     * @param pData �������̂��߂ɃR�s�[�����f�[�^�̃|�C���^
     * @param size �f�[�^�̃T�C�Y(�o�C�g�P��)
     */
    void updateIndexBuffer(void* pData, size_t size);
    /**
     * @bfief ���_�o�b�t�@�f�[�^���X�V����
     * @param pData �R�s�[�����f�[�^�̃|�C���^
     * @param offset �I�t�Z�b�g
     * @param size �f�[�^�̃T�C�Y(�o�C�g�P��)
     */
    void updateVertexBuffer(void* pData, size_t offset, size_t size);
    /**
     * @bfief �C���f�b�N�X�o�b�t�@�f�[�^���X�V����
     * @param pData �R�s�[�����f�[�^�̃|�C���^
     * @param offset �I�t�Z�b�g
     * @param size �f�[�^�̃T�C�Y(�o�C�g�P��)
     */
    void updateIndexBuffer(void* pData, size_t offset, size_t size);

    /**
     * @brief �ݒ肵�����_�����f�[�^���o�C���h����
     */
    void bindVertexBuffer();
    /**
     * @brief ���_�����f�[�^�̔z���ݒ肷��
     * @param name 
     * @param index ���_�����̃C���f�b�N�X
     * @param size ���_�����̃R���|�[�l���g���B1,2,3,4�ł���K�v������
     * @param needToBeNormallized �f�[�^�𐳋K������ꍇtrue�A���ڎw�肷��ꍇfalse
     * @param offset �o�b�t�@�f�[�^���̃I�t�Z�b�g
     * @attention ���̊֐����R�[������O�ɒ��_�o�b�t�@��ݒ肷��K�v������B
     */
    void setAttribute(const std::string& name, int index, int size, bool needToBeNormallized, std::size_t offset);

private:
    GLuint m_vertexArray;

    VertexBuffer* m_vertexBuffer;
    VertexBuffer* m_indexBuffer;

    unsigned int m_vertexCount;
    unsigned int m_indexCount;

    bool m_isVertexBufferCreated;

    std::size_t m_stride;
    std::unordered_map<std::string, Attribute> m_attributes;
};

NS_OCF_END
