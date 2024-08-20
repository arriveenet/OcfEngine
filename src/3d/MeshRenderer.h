#pragma once
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

protected:
    Mesh m_mesh;
};

NS_OCF_END