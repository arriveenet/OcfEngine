#include "UIMenu.h"
#include "UIMenuItem.h"
#include "2d/DrawShape.h"
#include "base/Types.h"
#include "platform/PlatformMacros.h"

NS_OCF_BEGIN

namespace ui {

Menu* Menu::create()
{
    Menu* menu = new Menu();
    if (menu->init()) {
        return menu;
    }
    OCF_SAFE_DELETE(menu);
    return nullptr;
}

Menu::Menu()
    : m_isVisible(false)
    , m_pBackground(nullptr)
{
}

Menu::~Menu()
{
    removeAllMenuItems();
}

bool Menu::init()
{
    if (Widget::init()) {
        setSize(MENU_DEFAULT_WIDTH, MENU_PADDING * 2);
        return true;
    }
    return false;
}

void Menu::addMenuItem(MenuItem* item)
{
    if (item == nullptr) {
        return;
    }

    m_menuItems.push_back(item);
    addChild(item);
    
    updateLayout();
}

void Menu::removeMenuItem(MenuItem* item)
{
    if (item == nullptr) {
        return;
    }

    auto it = std::find(m_menuItems.begin(), m_menuItems.end(), item);
    if (it != m_menuItems.end()) {
        removeChild(item);
        m_menuItems.erase(it);
        updateLayout();
    }
}

void Menu::removeAllMenuItems()
{
    for (MenuItem* item : m_menuItems) {
        removeChild(item);
    }
    m_menuItems.clear();
    updateLayout();
}

MenuItem* Menu::getMenuItem(size_t index) const
{
    if (index < m_menuItems.size()) {
        return m_menuItems[index];
    }
    return nullptr;
}

void Menu::setVisible(bool visible)
{
    m_isVisible = visible;
    Widget::setVisible(visible);
}

bool Menu::isVisible() const
{
    return m_isVisible;
}

void Menu::initRenderer()
{
    // Remove existing background
    if (m_pBackground) {
        removeChild(m_pBackground);
        m_pBackground = nullptr;
    }

    // Create menu background
    m_pBackground = DrawShape::create();
    addChild(m_pBackground);

    const glm::vec2 size = getSize();
    
    // Draw menu background with border
    m_pBackground->drawFillRect(glm::vec2(0, 0), size, glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));
    m_pBackground->drawRect(glm::vec2(0, 0), size, glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));
    
    // Add subtle shadow effect
    const float shadowOffset = 2.0f;
    m_pBackground->drawFillRect(
        glm::vec2(shadowOffset, -shadowOffset), 
        glm::vec2(size.x + shadowOffset, -shadowOffset + size.y), 
        glm::vec4(0.0f, 0.0f, 0.0f, 0.2f)
    );
}

void Menu::updateLayout()
{
    if (m_menuItems.empty()) {
        setSize(MENU_DEFAULT_WIDTH, MENU_PADDING * 2);
        return;
    }

    float maxWidth = MENU_DEFAULT_WIDTH;
    float totalHeight = MENU_PADDING;

    // Calculate required size
    for (MenuItem* item : m_menuItems) {
        if (item) {
            const glm::vec2 itemSize = item->getSize();
            maxWidth = std::max(maxWidth, itemSize.x);
            totalHeight += MENU_ITEM_HEIGHT;
        }
    }
    
    totalHeight += MENU_PADDING;

    // Set menu size
    setSize(maxWidth, totalHeight);

    // Position menu items
    float currentY = totalHeight - MENU_PADDING - MENU_ITEM_HEIGHT;
    for (MenuItem* item : m_menuItems) {
        if (item) {
            item->setSize(maxWidth - MENU_PADDING * 2, MENU_ITEM_HEIGHT);
            item->setPosition(MENU_PADDING, currentY);
            currentY -= MENU_ITEM_HEIGHT;
        }
    }

    // Update renderer with new size
    initRenderer();
}

} // namespace ui

NS_OCF_END