#pragma once
#include "base/types.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

NS_OCF_BEGIN

class Input {
public:
    static bool init();

    static void prepareUpdate();
    static void update();

    static ButtonState getKeyState(Keyboard::KeyCode keyCode);

    static glm::vec2 getMousePosition();
    static ButtonState getMouseButtonState(Mouse::MouseButton button);

private:
    static Keyboard m_keyboard;
    static Mouse m_mouse;
};

NS_OCF_END
