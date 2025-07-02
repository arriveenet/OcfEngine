#include "UIMenuItem.h"
#include "2d/DrawShape.h"
#include "2d/Label.h"
#include "base/Types.h"
#include "platform/PlatformMacros.h"

NS_OCF_BEGIN

namespace ui {

MenuItem* MenuItem::create(std::string_view text)
{
    MenuItem* menuItem = new MenuItem();
    if (menuItem->init()) {
        if (!text.empty()) {
            menuItem->setText(text);
        }
        return menuItem;
    }
    OCF_SAFE_DELETE(menuItem);
    return nullptr;
}

MenuItem::MenuItem()
    : m_enabled(true)
    , m_separator(false)
    , m_highlighted(false)
{
}

MenuItem::~MenuItem()
{
}

bool MenuItem::init()
{
    if (ButtonBase::init()) {
        setSize(MENU_ITEM_DEFAULT_WIDTH, MENU_ITEM_DEFAULT_HEIGHT);
        return true;
    }
    return false;
}

void MenuItem::setEnabled(bool enabled)
{
    if (m_enabled != enabled) {
        m_enabled = enabled;
        // Update visual state
        if (m_pTextRenderer) {
            // Set text opacity based on enabled state
            m_pTextRenderer->setOpacity(m_enabled ? 1.0f : 0.5f);
        }
    }
}

void MenuItem::setSeparator(bool separator)
{
    m_separator = separator;
    // Trigger renderer update if needed
    initRenderer();
}

void MenuItem::initRenderer()
{
    // Remove existing renderer components
    if (m_pButtonBackground) {
        removeChild(m_pButtonBackground);
        m_pButtonBackground = nullptr;
    }

    // Create background
    m_pButtonBackground = DrawShape::create();
    addChild(m_pButtonBackground);

    // Set background based on state
    const glm::vec2 size = getSize();
    if (m_highlighted && m_enabled) {
        m_pButtonBackground->drawFillRect(glm::vec2(0, 0), size, glm::vec4(0.2f, 0.5f, 0.8f, 0.8f));
    } else {
        m_pButtonBackground->drawFillRect(glm::vec2(0, 0), size, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f));
    }

    // Add separator line if needed
    if (m_separator) {
        const float separatorY = -2.0f;
        m_pButtonBackground->drawLine(
            glm::vec2(4.0f, separatorY), 
            glm::vec2(size.x - 4.0f, separatorY), 
            Color4f::GRAY
        );
    }
}

void MenuItem::updateTextLocation()
{
    if (m_pTextRenderer) {
        const glm::vec2 size = getSize();
        const glm::vec2 textSize = m_pTextRenderer->getSize();
        
        // Center text vertically, add left padding
        const float textX = MENU_ITEM_TEXT_PADDING;
        const float textY = (size.y - textSize.y) * 0.5f;
        
        m_pTextRenderer->setPosition(textX, textY);
    }
}

void MenuItem::onMouseClicked()
{
    if (m_enabled && m_onAction) {
        m_onAction();
    }
}

void MenuItem::onMousePressed()
{
    if (m_enabled) {
        m_highlighted = true;
        initRenderer();
    }
}

void MenuItem::onMouseReleased()
{
    if (m_highlighted) {
        m_highlighted = false;
        initRenderer();
    }
}

} // namespace ui

NS_OCF_END