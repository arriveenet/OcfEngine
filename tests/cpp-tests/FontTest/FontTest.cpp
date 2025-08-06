#include "FontTest.h"
#include "renderer/Image.h"
#include "base/EventListenerKeyboard.h"
#include "ui/UICheckBox.h"
#include "ui/UIRadioButton.h"

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

FontTest::FontTest()
    : m_pLabel(nullptr)
    , m_pDrawShape(nullptr)
    , m_index(0)
{
}

bool FontTest::init()
{
    Viewport* root = getRoot();

    m_pLabel = ocf::Label::createWithTTF("fonts/NotoSansJP-Regular.ttf", "こんにちは、世界!", 24);
    root->addChild(m_pLabel);
    m_pLabel->setPosition(glm::vec2(100, 100));

    m_pDrawShape = DrawShape::create();
    root->addChild(m_pDrawShape);

    m_maxRectsBinPack.init(512, 512);
    m_maxRectsBinPack.setHeuristic(MaxRectsBinPack::FreeRectChoiceHeuristic::RectBestsShortSideFit);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->m_onKeyPressed = [&](Keyboard::KeyCode key, Event* /* pEvent */) {
        if (key == Keyboard::KeyCode::KEY_SPACE) {
            if (m_index > std::size(rects) - 1)
                return;

            Rect rect = m_maxRectsBinPack.insert(static_cast<float>(rects[m_index].x), 
                                                 static_cast<float>(rects[m_index].y));
            OCFLOG("Packed to (x,y)=(%f,%f), (w,h)=(%f,%f)\n", rect.m_position.x, rect.m_position.y, rect.m_size.x, rect.m_size.y);
            m_index++;
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

    m_pGame->getEventDispatcher()->addEventListener(keyboardListener, m_pDrawShape);

    auto checkBox = ui::CheckBox::create("Text");
    checkBox->setPosition(glm::vec2(200, 200));
    root->addChild(checkBox);

    auto group = std::make_shared<ui::ToggleGroup>();

    auto radioButton1 = ui::RadioButton::create("option1");
    radioButton1->setPosition(glm::vec2(500, 300));
    radioButton1->setToggleGroup(group);
    radioButton1->setSelected(true);
    root->addChild(radioButton1);

    auto radioButton2 = ui::RadioButton::create("option2");
    radioButton2->setPosition(glm::vec2(500, 270));
    radioButton2->setToggleGroup(group);
    root->addChild(radioButton2);

    auto radioButton3 = ui::RadioButton::create("option3");
    radioButton3->setPosition(glm::vec2(500, 240));
    radioButton3->setToggleGroup(group);
    root->addChild(radioButton3);

    auto button = ui::Button::create("button");
    button->setPosition(glm::vec2(500, 200));
    root->addChild(button);

    return TestCase::init();
}

void FontTest::onExit()
{
    m_index = 0;
    TestCase::onExit();
}
