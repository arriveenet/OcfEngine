#pragma once
#include "../BaseTest.h"

class MeshRendererTest : public TestCase {
public:
    MeshRendererTest();
    ~MeshRendererTest();

    bool init() override;
    
    void updateNode(float deltaTime) override;

    ocf::Camera* m_pCamera;
};