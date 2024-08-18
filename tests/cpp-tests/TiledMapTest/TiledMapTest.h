#pragma once
#include "../BaseTest.h"

class TiledMapTest : public TestCase {
public:
    TiledMapTest();
    ~TiledMapTest();

    bool init() override;
    void processInput(const ocf::InputState& inputState) override;

protected:
    ocf::Camera* m_camera;
};