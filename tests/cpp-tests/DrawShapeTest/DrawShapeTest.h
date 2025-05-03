#pragma once
#include "../BaseTest.h"

class DrawShapeTest : public TestCase {
public:
    void onEnter() override;

    ocf::DrawShape* m_pDrawShape;
};