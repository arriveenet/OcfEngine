#pragma once
#include "../BaseTest.h"
#include "ocf/3d/FirstPersonCamera.h"

class MeshRendererTest : public TestCase {
public:
    MeshRendererTest();
    ~MeshRendererTest();

    bool init() override;
    
    void updateScene(float deltaTime) override;

    ocf::FirstPersonCamera* m_pCamera;
    ocf::Label* m_pStatusLabel;
};