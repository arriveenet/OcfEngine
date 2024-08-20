#pragma once
#include <vector>
#include <string>
#include "base/types.h"
#include "base/GameObject.h"

NS_OCF_BEGIN

class Mesh : public GameObject {
    friend class MeshRenderer;
public:
    Mesh();
    virtual ~Mesh();

protected:
    std::vector<Vertex3fN3fT2f> m_data;
};

NS_OCF_END