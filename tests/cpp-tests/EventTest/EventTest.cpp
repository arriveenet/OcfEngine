#include "EventTest.h"
#include "base/EventDispatcher.h"
#include "base/EventListenerKeyboard.h"
#include "base/EventListenerMouse.h"

USING_NS_OCF;

EventTest::EventTest()
    : m_pKeyboardLabel(nullptr)
    , m_pMousePositionLabel(nullptr)
{
    setTestCaseName("EventTest");
}

bool EventTest::init()
{
    if (!TestCase::init())
        return false;

    Viewport* root = getRoot();

    m_pKeyboardLabel = Label::create("Key code: ");
    m_pKeyboardLabel->setPosition(glm::vec2(40.0f, 550.f));
    root->addChild(m_pKeyboardLabel);

    m_pMousePositionLabel = Label::create("Mouse position: ");
    m_pMousePositionLabel->setPosition(glm::vec2(40.0f, 500.0f));
    root->addChild(m_pMousePositionLabel);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->m_onKeyPressed = std::bind(&EventTest::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
    keyboardListener->m_onKeyReleased = std::bind(&EventTest::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
    m_pGame->getEventDispatcher()->addEventListener(keyboardListener, m_pKeyboardLabel);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->m_onMouseMove = std::bind(&EventTest::onMouseMove, this, std::placeholders::_1);
    m_pGame->getEventDispatcher()->addEventListener(mouseListener, m_pMousePositionLabel);

    return true;
}

void EventTest::onKeyPressed(Keyboard::KeyCode key, Event* /* pEvent */)
{
    char str[256];
    snprintf(str, sizeof(str), "Key pressed code: %s",
             ocf::Keyboard::getString(key).c_str());
    m_pKeyboardLabel->setString(str);
}

void EventTest::onKeyReleased(ocf::Keyboard::KeyCode key, ocf::Event* /* pEvent */)
{
    char str[256];
    snprintf(str, sizeof(str), "Key released code: %s",
             ocf::Keyboard::getString(key).c_str());
    m_pKeyboardLabel->setString(str);
}

void EventTest::onMouseMove(ocf::Event* pEvent)
{
    EventMouse* mouseEvent = static_cast<EventMouse*>(pEvent);

    char str[256];
    snprintf(str, sizeof(str), "Mouse position: %.1f, %.1f", mouseEvent->getPosition().x, mouseEvent->getPosition().y);
    m_pMousePositionLabel->setString(str);
}
