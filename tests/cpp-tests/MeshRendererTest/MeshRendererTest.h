#pragma once
#include "../BaseTest.h"

class MeshRendererTest : public TestCase {
public:
    MeshRendererTest();
    ~MeshRendererTest();

    bool init() override;

    void processInput(const ocf::InputState& inputState) override;

    ocf::Camera* m_pCamera;
};