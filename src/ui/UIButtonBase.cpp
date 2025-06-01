#include "UIButtonBase.h"
#include "2d/Camera.h"
#include "2d/Label.h"
#include "base/Rect.h"

NS_OCF_BEGIN

namespace ui {

ButtonBase::ButtonBase()
    : m_pButtonBackground(nullptr)
    , m_pCheckMark(nullptr)
    , m_pTextRenderer(nullptr)
    , m_onAction(nullptr)
    , m_isSelected(false)
{}

ButtonBase::~ButtonBase() {}

void ButtonBase::updateNode(float /*deltaTime*/)
{
    glm::vec2 mousePos = Input::getMousePosition();
    Camera* camera = Camera::getDefaultCamera();
    Rect contentRect(0.0f, 0.0f, m_size.x, m_size.y);

    if (isScreenPointInRect(mousePos, camera, getWorldToNodeTransform(), contentRect, nullptr)) {
        if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Pressed) {
            m_pressed = true;
            onMousePressed();
        }
        else if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Released) {
            onMouseReleased();
            if (m_pressed) {
                m_pressed = false;
                onMouseClicked();
            }
        }

        m_focus = true;
    }
    else {
        m_focus = false;
        m_pressed = false;
    }

    }

void ButtonBase::setOnAction(ActionEvent value)
{
    m_onAction = value;
}

void ButtonBase::setText(std::string_view text)
{
    if (text.compare(getText()) == 0) {
        return;
    }

    createTextRendererIfNull();

    m_pTextRenderer->setString(text);
    m_pTextRenderer->update(0.0f);

    updateTextLocation();
}

std::string ButtonBase::getText() const
{
    if (m_pTextRenderer == nullptr) {
        return "";
    }

    return m_pTextRenderer->getString();
}

bool ButtonBase::createTextRendererIfNull()
{
    if (m_pTextRenderer == nullptr) {
        m_pTextRenderer = Label::createWithTTF("fonts/NotoSansJP-Regular.ttf", "", 16);
        addChild(m_pTextRenderer);

        return true;
    }

    return false;
}

void ButtonBase::updateTextLocation()
{
}

} // namespace ui

NS_OCF_END