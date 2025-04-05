#include "FontTest.h"
#include "renderer/Image.h"
#include "base/EventListenerKeyboard.h"

using namespace ocf;

static glm::ivec2 rects[] = {
    { 200, 200 },
    { 100, 100 },
    { 150, 200 },
    { 200, 150 },
    { 100, 150 },
    { 150, 100 },
    { 64, 64 },
    { 64, 128 },
    { 32, 32 },
    { 16, 16 },
    { 32, 32 },
    { 32, 32 },
    { 32, 32 },
    { 32, 32 },
};

static size_t index = 0;

FontTest::FontTest()
    : m_pLabel(nullptr)
    , m_pDrawShape(nullptr)
{
}

bool FontTest::init()
{
    if (!TestCase::init())
        return false;

    m_pLabel = ocf::Label::createWithTTF("fonts/cour.ttf", "Hello, World!", 24);
    addChild(m_pLabel);
    m_pLabel->setPosition(100, 100);

    m_pDrawShape = DrawShape::create();
    addChild(m_pDrawShape);

    m_maxRectsBinPack.init(512, 512);
    m_maxRectsBinPack.setHeuristic(MaxRectsBinPack::FreeRectChoiceHeuristic::RectBestsShortSideFit);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->m_onKeyPressed = [&](Keyboard::KeyCode key, Event* /* pEvent */) {
        if (key == Keyboard::KeyCode::KEY_SPACE) {
            if (index > std::size(rects) - 1)
                return;

            Rect rect = m_maxRectsBinPack.insert(static_cast<float>(rects[index].x), 
                                                 static_cast<float>(rects[index].y));
            OCFLOG("Packed to (x,y)=(%f,%f), (w,h)=(%f,%f)\n", rect.m_position.x, rect.m_position.y, rect.m_size.x, rect.m_size.y);
            index++;
        }
        m_pDrawShape->clear();

        for (const auto& rect : m_maxRectsBinPack.getUsedRects()) {
            m_pDrawShape->drawRect(glm::vec2(rect.m_position.x - 1.f, rect.m_position.y - 1.f),
                glm::vec2(rect.getMaxX() - 1.f, rect.getMaxY()) - 1.f, Color4f::ORANGE);
        }

        for (const auto& rect : m_maxRectsBinPack.getFreeRects()) {
            m_pDrawShape->drawRect(glm::vec2(rect.m_position.x - 1.f, rect.m_position.y - 1.f),
                glm::vec2(rect.getMaxX() - 1.f, rect.getMaxY()) - 1.f, Color4f::GRAY);
        }

        m_pDrawShape->drawRect(glm::vec2(0, 0), glm::vec2(512, 512), Color4f::GREEN);
        };

    m_pGame->getEventDispatcher()->addEventListener(keyboardListener, this);

    return true;
}

void FontTest::onExit()
{
    index = 0;
    TestCase::onExit();
}
