#pragma once
#include "../BaseTest.h"

class EventTest : public TestCase {
public:
    EventTest();

    bool init() override;

    void onKeyPressed(ocf::Keyboard::KeyCode key, ocf::Event* pEvent);

    void onKeyReleased(ocf::Keyboard::KeyCode key, ocf::Event* pEvent);

    void onMouseMove(ocf::Event* pEvent);

    void onMouseScroll(ocf::Event* pEvent);

    ocf::Label* m_pKeyboardLabel;
    ocf::Label* m_pMousePositionLabel;
    ocf::Label* m_pMouseScrollLabel;
};