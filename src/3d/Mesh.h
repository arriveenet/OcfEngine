#pragma once
#include <vector>
#include <string>
#include "base/types.h"
#include "base/GameObject.h"
#include "renderer/MeshCommand.h"

NS_OCF_BEGIN

class Texture2D;
class Renderer;

class Mesh : public GameObject {
    friend class MeshRenderer;
public:
    Mesh();
    virtual ~Mesh();

    bool setupMesh();

    void draw(Renderer* renderer, float globalZOrder, const glm::mat4& transform);

    void setTexture(std::string_view fileName);
    Texture2D* getTexture() const { return m_pTexture; }

protected:
    std::vector<glm::vec3> m_data;
    Texture2D* m_pTexture;
    MeshCommand m_meshCommand;
};

NS_OCF_END