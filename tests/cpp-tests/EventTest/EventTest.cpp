#include "EventTest.h"
#include "base/EventDispatcher.h"
#include "base/EventListenerKeyboard.h"
#include "base/EventListenerMouse.h"

USING_NS_OCF;

EventTest::EventTest()
{
    setTestCaseName("EventTest");
}

bool EventTest::init()
{
    if (!TestCase::init())
        return false;

    m_pKeyboardLabel = Label::create("Key code: ");
    m_pKeyboardLabel->setPosition(40.0f, 550.f);
    addChild(m_pKeyboardLabel);

    m_pMousePositionLabel = Label::create("Mouse position: ");
    m_pMousePositionLabel->setPosition(40.0f, 500.0f);
    addChild(m_pMousePositionLabel);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->m_onKeyPressed = std::bind(&EventTest::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
    keyboardListener->m_onKeyReleased = std::bind(&EventTest::onKeyReleased, this, std::placeholders::_1, std::placeholders::_2);
    m_pGame->getEventDispatcher()->addEventListener(keyboardListener, this);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->m_onMouseMove = std::bind(&EventTest::onMouseMove, this, std::placeholders::_1);
    m_pGame->getEventDispatcher()->addEventListener(mouseListener, this);

    return true;
}

void EventTest::onKeyPressed(Keyboard::KeyCode key, Event* pEvent)
{
    char str[256];
    sprintf_s(str, "Key pressed code: %d", (int)key);
    m_pKeyboardLabel->setString(str);
}

void EventTest::onKeyReleased(ocf::Keyboard::KeyCode key, ocf::Event* pEvent)
{
    char str[256];
    sprintf_s(str, "Key released code: %d", (int)key);
    m_pKeyboardLabel->setString(str);
}

void EventTest::onMouseMove(ocf::Event* pEvent)
{
    EventMouse* mouseEvent = static_cast<EventMouse*>(pEvent);

    char str[256];
    sprintf_s(str, "Mouse position: %.1f, %.1f", mouseEvent->getPosition().x, mouseEvent->getPosition().y);
    m_pMousePositionLabel->setString(str);
}
