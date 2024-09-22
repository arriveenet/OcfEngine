#include "EventTest.h"
#include "base/EventDispatcher.h"
#include "base/EventListenerKeyboard.h"

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
    addChild(m_pKeyboardLabel);

    auto listener = EventListenerKeyboard::create();
    listener->m_onKeyPressed = [=](Keyboard::KeyCode key, Event* pEvent) {
        char str[256];
        sprintf_s(str, "Key code: %d", (int)key);
        m_pKeyboardLabel->setString(str);
        };


    m_pGame->getEventDispatcher()->addEventListener(listener, this);

    return true;
}

void EventTest::onKeyPressed(Keyboard::KeyCode key, Event* pEvent)
{
}
