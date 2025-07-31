#pragma once
#include "base/Types.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

NS_OCF_BEGIN

class Input {
public:
    enum class MouseMode {
        Normal,
        Hidden,
        Captured
    };

    static bool init();

    static void prepareUpdate();
    static void update();

    static ButtonState getKeyState(Keyboard::KeyCode keyCode);

    static glm::vec2 getMousePosition();
    static ButtonState getMouseButtonState(Mouse::MouseButton button);
    static void setMouseMode(MouseMode mode);
    static MouseMode getMouseMode();

private:
    static Keyboard m_keyboard;
    static Mouse m_mouse;
    static MouseMode m_mouseMode;
};

NS_OCF_END
