#pragma once
#include "../BaseTest.h"

class MeshRendererTest : public TestCase {
public:
    MeshRendererTest();
    ~MeshRendererTest();

    bool init() override;

    ocf::Camera* m_pCamera;
};