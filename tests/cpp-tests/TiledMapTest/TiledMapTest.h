#pragma once
#include "../BaseTest.h"

class TiledMapTest : public TestCase {
public:
    TiledMapTest();
    ~TiledMapTest();

    bool init() override;

protected:
    ocf::Camera2D* m_camera;
};