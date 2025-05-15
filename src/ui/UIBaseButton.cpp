#include "UIBaseButton.h"
#include "2d/Camera.h"
#include "base/Rect.h"

NS_OCF_BEGIN

namespace ui {

BaseButton::BaseButton() {}

BaseButton::~BaseButton() {}

void BaseButton::updateState()
{
    glm::vec2 mousePos = Input::getMousePosition();
    Camera *camera = Camera::getDefaultCamera();
    Rect rect(0, 0, m_size.x, m_size.y);

    if (isScreenPointInRect(mousePos, camera, getWorldToNodeTransform(), rect, nullptr)) {
        m_isHit = true;
    }
    else {
        m_isHit = false;
    }
}


} // namespace ui

NS_OCF_END