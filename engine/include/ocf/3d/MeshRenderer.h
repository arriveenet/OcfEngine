#pragma once
#include "ocf/3d/Mesh.h"
#include "ocf/scene/Node.h"
#include <string>
#include <vector>

namespace ocf {

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

} // namespace ocf