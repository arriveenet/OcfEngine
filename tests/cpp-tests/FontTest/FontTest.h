#pragma once
#include "../BaseTest.h"
#include "base/MaxRectsBinPack.h"

class FontTest : public TestCase {
public:
    FontTest();

    bool init() override;

    void onExit() override;

private:
    ocf::Label* m_pLabel;
    ocf::DrawShape* m_pDrawShape;
    ocf::MaxRectsBinPack m_maxRectsBinPack;
    size_t m_index;
};