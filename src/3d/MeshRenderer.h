#pragma once
#include <vector>
#include <string>
#include "2d/Node.h"
#include "3d/Mesh.h"

NS_OCF_BEGIN

class MeshRenderer : public Node {
public:
    static MeshRenderer* create(std::string_view fileName);

    MeshRenderer();
    virtual ~MeshRenderer();

    bool initWithObjFile(std::string_view objFile);

    void draw(Renderer* renderer, const glm::mat4& transform) override;

protected:
    std::vector<Mesh* >m_meshes;
};

NS_OCF_END