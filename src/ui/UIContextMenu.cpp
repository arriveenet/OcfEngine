#include "UIContextMenu.h"
#include "base/Game.h"
#include "platform/GLView.h"
#include "base/Types.h"
#include "platform/PlatformMacros.h"
#include "2d/DrawShape.h"

NS_OCF_BEGIN

namespace ui {

ContextMenu* ContextMenu::create()
{
    ContextMenu* contextMenu = new ContextMenu();
    if (contextMenu->init()) {
        return contextMenu;
    }
    OCF_SAFE_DELETE(contextMenu);
    return nullptr;
}

ContextMenu::ContextMenu()
    : m_autoHide(true)
    , m_showPosition(0.0f, 0.0f)
{
}

ContextMenu::~ContextMenu()
{
}

bool ContextMenu::init()
{
    if (Menu::init()) {
        // Context menus start hidden
        setVisible(false);
        return true;
    }
    return false;
}

void ContextMenu::showAt(const glm::vec2& position)
{
    m_showPosition = position;
    
    // Update layout to ensure proper sizing
    updateLayout();
    
    // Adjust position to stay within screen bounds
    const glm::vec2 adjustedPosition = adjustPositionForBounds(position);
    setPosition(adjustedPosition);
    
    // Show the menu
    setVisible(true);
}

void ContextMenu::hide()
{
    setVisible(false);
}

void ContextMenu::updateNode(float deltaTime)
{
    Menu::updateNode(deltaTime);
    
    // TODO: Handle auto-hide functionality when clicking outside
    // This would require checking mouse events against menu bounds
    // For now, we'll implement basic functionality
}

void ContextMenu::initRenderer()
{
    Menu::initRenderer();
    
    // Context menus can have additional visual styling
    // like drop shadows or different border styles
    if (m_pBackground) {
        const glm::vec2 size = getSize();
        
        // Add a more pronounced shadow for popup appearance
        const float shadowOffset = 3.0f;
        m_pBackground->drawFillRect(
            glm::vec2(shadowOffset, -shadowOffset), 
            glm::vec2(size.x + shadowOffset, -shadowOffset + size.y), 
            glm::vec4(0.0f, 0.0f, 0.0f, 0.3f)
        );
    }
}

glm::vec2 ContextMenu::adjustPositionForBounds(const glm::vec2& position)
{
    glm::vec2 adjustedPos = position;
    const glm::vec2 menuSize = getSize();
    
    // Get screen dimensions - we'll need to access this from the current view
    // For now, we'll use reasonable default bounds
    // TODO: Get actual screen bounds from GLView or similar
    const glm::vec2 screenSize(1024.0f, 768.0f); // Default fallback
    
    // Adjust X position to keep menu on screen
    if (adjustedPos.x + menuSize.x > screenSize.x) {
        adjustedPos.x = screenSize.x - menuSize.x - CONTEXT_MENU_MARGIN;
    }
    if (adjustedPos.x < CONTEXT_MENU_MARGIN) {
        adjustedPos.x = CONTEXT_MENU_MARGIN;
    }
    
    // Adjust Y position to keep menu on screen
    if (adjustedPos.y + menuSize.y > screenSize.y) {
        adjustedPos.y = screenSize.y - menuSize.y - CONTEXT_MENU_MARGIN;
    }
    if (adjustedPos.y < CONTEXT_MENU_MARGIN) {
        adjustedPos.y = CONTEXT_MENU_MARGIN;
    }
    
    return adjustedPos;
}

} // namespace ui

NS_OCF_END