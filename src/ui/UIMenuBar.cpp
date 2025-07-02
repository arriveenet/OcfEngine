#include "UIMenuBar.h"
#include "UIMenu.h"
#include "2d/DrawShape.h"
#include "2d/Label.h"
#include "base/Types.h"
#include "platform/PlatformMacros.h"

NS_OCF_BEGIN

namespace ui {

MenuBar* MenuBar::create()
{
    MenuBar* menuBar = new MenuBar();
    if (menuBar->init()) {
        return menuBar;
    }
    OCF_SAFE_DELETE(menuBar);
    return nullptr;
}

MenuBar::MenuBar()
    : m_menuBarHeight(MENU_BAR_DEFAULT_HEIGHT)
    , m_activeMenuIndex(-1)
    , m_pBackground(nullptr)
{
}

MenuBar::~MenuBar()
{
    removeAllMenus();
}

bool MenuBar::init()
{
    if (Widget::init()) {
        setSize(400.0f, m_menuBarHeight); // Default width, will be adjusted
        return true;
    }
    return false;
}

void MenuBar::addMenu(std::string_view title, Menu* menu)
{
    if (menu == nullptr) {
        return;
    }

    // Create menu entry
    MenuEntry entry(title, menu);
    
    // Create title widget (button-like widget for the menu title)
    entry.titleWidget = new Widget();
    if (entry.titleWidget && entry.titleWidget->init()) {
        addChild(entry.titleWidget);
        
        // Add text to title widget
        auto titleLabel = Label::createWithTTF("fonts/NotoSansJP-Regular.ttf", std::string(title), 16);
        if (titleLabel) {
            entry.titleWidget->addChild(titleLabel);
        }
    } else {
        OCF_SAFE_DELETE(entry.titleWidget);
        return;
    }
    
    // Add menu as child but keep it hidden initially
    addChild(menu);
    menu->setVisible(false);
    
    m_menus.push_back(entry);
    updateLayout();
}

void MenuBar::removeMenu(std::string_view title)
{
    auto it = std::find_if(m_menus.begin(), m_menus.end(),
        [title](const MenuEntry& entry) {
            return entry.title == title;
        });
    
    if (it != m_menus.end()) {
        // Clean up widgets
        if (it->titleWidget) {
            removeChild(it->titleWidget);
        }
        if (it->menu) {
            removeChild(it->menu);
        }
        
        m_menus.erase(it);
        updateLayout();
    }
}

void MenuBar::removeAllMenus()
{
    for (auto& entry : m_menus) {
        if (entry.titleWidget) {
            removeChild(entry.titleWidget);
        }
        if (entry.menu) {
            removeChild(entry.menu);
        }
    }
    m_menus.clear();
    m_activeMenuIndex = -1;
    updateLayout();
}

void MenuBar::setMenuBarHeight(float height)
{
    m_menuBarHeight = height;
    updateLayout();
}

void MenuBar::initRenderer()
{
    // Remove existing background
    if (m_pBackground) {
        removeChild(m_pBackground);
        m_pBackground = nullptr;
    }

    // Create menu bar background
    m_pBackground = DrawShape::create();
    addChild(m_pBackground);

    const glm::vec2 size = getSize();
    
    // Draw menu bar background
    m_pBackground->drawFillRect(glm::vec2(0, 0), size, glm::vec4(0.9f, 0.9f, 0.9f, 1.0f));
    
    // Add bottom border
    m_pBackground->drawLine(
        glm::vec2(0, 0), 
        glm::vec2(size.x, 0), 
        glm::vec4(0.6f, 0.6f, 0.6f, 1.0f)
    );
}

void MenuBar::updateLayout()
{
    if (m_menus.empty()) {
        setSize(400.0f, m_menuBarHeight);
        initRenderer();
        return;
    }

    float totalWidth = 0.0f;
    float currentX = 0.0f;

    // Calculate positions for menu titles
    for (auto& entry : m_menus) {
        if (entry.titleWidget) {
            // Set title widget size and position
            const float titleWidth = entry.title.length() * 8.0f + MENU_TITLE_PADDING * 2; // Rough text width calculation
            entry.titleWidget->setSize(titleWidth, MENU_TITLE_HEIGHT);
            entry.titleWidget->setPosition(currentX, (m_menuBarHeight - MENU_TITLE_HEIGHT) * 0.5f);
            
            currentX += titleWidth;
            totalWidth += titleWidth;
        }
    }

    // Set menu bar size
    setSize(std::max(400.0f, totalWidth), m_menuBarHeight);

    // Position dropdown menus
    currentX = 0.0f;
    for (auto& entry : m_menus) {
        if (entry.menu && entry.titleWidget) {
            const float titleWidth = entry.titleWidget->getSize().x;
            
            // Position menu below the title
            entry.menu->setPosition(currentX, -entry.menu->getSize().y);
            
            currentX += titleWidth;
        }
    }

    initRenderer();
}

void MenuBar::onMouseClicked()
{
    // TODO: Implement menu opening/closing logic based on mouse position
    // This would involve checking which menu title was clicked
    // and toggling the corresponding dropdown menu
}

} // namespace ui

NS_OCF_END