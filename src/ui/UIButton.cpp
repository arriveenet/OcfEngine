#include "UIButton.h"
#include "2d/Sprite.h"
#include "2d/DrawShape.h"
#include "2d/Label.h"

NS_OCF_BEGIN

namespace ui {

Button* Button::create()
{
    Button* button = new Button();
    if (button->init()) {
        return button;
    }
    OCF_SAFE_DELETE(button);
    return nullptr;
}

Button* Button::create(std::string_view normalImage, std::string_view activeImage)
{
    Button* button = new Button();
    if (button->init(normalImage, activeImage)) {
        return button;
    }
    OCF_SAFE_DELETE(button);
    return nullptr;
}

Button::Button()
    : m_pButtonNormalRenderer(nullptr)
    , m_pButtonActiveRenderer(nullptr)
    , m_pTextRenderer(nullptr)
    , m_onClick(nullptr)
{
}

Button::~Button()
{
}

bool Button::init()
{
    if (Widget::init()) {
        return true;
    }
    return false;
}

bool Button::init(std::string_view normalImage, std::string_view activeImage)
{
    m_normalFilename = normalImage;
    m_activeFilename = activeImage;

    if (Widget::init()) {
        return true;
    }
    return false;
}

void Button::updateNode(float deltaTime)
{
    glm::vec2 mousePos = Input::getMousePosition();
    if (this->hitTest(mousePos)) {
        m_pButtonNormalRenderer->setVisible(false);
        m_pButtonActiveRenderer->setVisible(true);
        if (Input::getMouseButtonState(Mouse::Left) == ButtonState::Pressed) {
            m_onClick();
        }
    }
    else {
        m_pButtonNormalRenderer->setVisible(true);
        m_pButtonActiveRenderer->setVisible(false);
    }
}

void Button::setOnClickCallback(std::function<void()> onClick)
{
    m_onClick = onClick;
}

void Button::setText(const std::string& text)
{
    if (text.compare(getText()) == 0) {
        return;
    }

    createTextRendererIfNull();

    m_pTextRenderer->setString(text);
    m_pTextRenderer->update(0.0f);

    updateTextLocation();
}

std::string Button::getText() const
{
    if (m_pTextRenderer == nullptr) {
        return "";
    }

    return m_pTextRenderer->getString();
}

void Button::initRenderer()
{
    m_pButtonNormalRenderer = Sprite::create(m_normalFilename);
    m_pButtonActiveRenderer = Sprite::create(m_activeFilename);

    m_pButtonNormalRenderer->setAnchorPoint(glm::vec2(0.0f, 0.0f));
    m_pButtonActiveRenderer->setAnchorPoint(glm::vec2(0.0f, 0.0f));

    addChild(m_pButtonNormalRenderer);
    addChild(m_pButtonActiveRenderer);

    setSize(m_pButtonNormalRenderer->getSize());
}

bool Button::createTextRendererIfNull()
{
    if (m_pTextRenderer == nullptr) {
        m_pTextRenderer = Label::create("");
        addChild(m_pTextRenderer);

        return true;
    }

    return false;
}

void Button::updateTextLocation()
{
    float x = (m_size.x - m_pTextRenderer->getSize().x) / 2.0f;
    glm::vec2 pos = (m_size - m_pTextRenderer->getSize()) / 2.0f;
    m_pTextRenderer->setPosition(pos);
}

}

NS_OCF_END
