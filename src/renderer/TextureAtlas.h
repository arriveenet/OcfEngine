#pragma once
#include <string>
#include "base/GameObject.h"
#include "Texture2D.h"
#include "base/Types.h"

NS_OCF_BEGIN

class TextureAtlas : public GameObject {
public:
    static TextureAtlas* create(const std::string& filename, size_t capacity);

    TextureAtlas();
    virtual ~TextureAtlas();

    bool initWithFile(const std::string& filename, size_t capacity);
    bool initWidthTexture(Texture2D* pTexture, size_t capacity);

    void insertQuad(QuadV3fT2f* pQuad, size_t index);
    void removeQuad(size_t index);

    size_t getTotalQuads() const;
    size_t getCapacity() const;
    Texture2D* getTexture() const;
    QuadV3fT2f* getQuads() const;
    unsigned short* getIndices() const { return m_pIndices; }

private:
    void setupIndices();

protected:
    size_t m_capacity = 0;
    size_t m_totalQuads = 0;
    Texture2D* m_pTexture = nullptr;
    QuadV3fT2f* m_pQuads = nullptr;
    unsigned short* m_pIndices = nullptr;
};

NS_OCF_END
