#include "FontTest.h"
#include "renderer/Image.h"

using namespace ocf;

FontTest::FontTest()
    : m_pLabel(nullptr)
{
}

bool FontTest::init()
{
    if (!TestCase::init())
        return false;

    m_pLabel = ocf::Label::createWithTTF("fonts/cour.ttf", "Hello, World!", 24);
    addChild(m_pLabel);

    return true;
}
