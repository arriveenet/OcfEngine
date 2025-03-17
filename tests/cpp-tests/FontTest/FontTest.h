#pragma once
#include "../BaseTest.h"

class FontTest : public TestCase {
public:
    FontTest();

    bool init() override;

    ocf::Label* m_pLabel;
};