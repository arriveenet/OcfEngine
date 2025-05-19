#include "UIButtonBase.h"
#include "2d/Camera.h"
#include "base/Rect.h"
#include "UIButtonBase.h"

NS_OCF_BEGIN

namespace ui {

ButtonBase::ButtonBase()
    : m_pButtonBackground(nullptr)
    , m_onAction(nullptr)
{}

ButtonBase::~ButtonBase() {}

void ButtonBase::updateNode(float /*deltaTime*/)
{
    glm::vec2 mousePos = Input::getMousePosition();
    Camera* camera = Camera::getDefaultCamera();
    Rect contentRect(0.0f, 0.0f, m_size.x, m_size.y);

    if (isScreenPointInRect(mousePos, camera, getWorldToNodeTransform(), contentRect, nullptr)) {
        if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Pressed) {
            onMousePressed();
        }
        else if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Released) {
            onMouseReleased();
        }

        m_focus = true;
    }
    else {
        m_focus = false;
    }

    }

void ButtonBase::setOnAction(ActionEvent value)
{
    m_onAction = value;
}

} // namespace ui

NS_OCF_END