#include "FontTest.h"

FontTest::FontTest()
    : m_pLabel(nullptr)
{
}

bool FontTest::init()
{
    if (!TestCase::init())
        return false;

    m_pLabel = ocf::Label::createWithTTF("fonts/cour.ttf", "Hello, World!", 24);

    return true;
}
