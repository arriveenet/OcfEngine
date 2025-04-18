#include "TrianglesCommand.h"
#define XXH_INLINE_ALL
#include "xxhash.h"

NS_OCF_BEGIN

TrianglesCommand::TrianglesCommand()
    : m_materialID(0)
    , m_texture(nullptr)
{
    m_Type = Type::TrianglesCommand;
}

TrianglesCommand::~TrianglesCommand()
{
}

void TrianglesCommand::init(float globalZOrder, Texture2D* texture, const Triangles& triangles, const glm::mat4& modelView)
{
    RenderCommand::init(globalZOrder, modelView);

    m_texture = texture;
    m_triangles = triangles;

    genarateMaterialID();
}

void TrianglesCommand::genarateMaterialID()
{
    if (m_texture == nullptr) {
        m_materialID = 0;
        return;
    }

    m_materialID = XXH32(m_texture, sizeof(Texture2D), 0);
}

NS_OCF_END
