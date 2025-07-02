#pragma once
#include "ui/UIWidget.h"
#include <vector>

NS_OCF_BEGIN

class DrawShape;

namespace ui {

class Menu;

/**
 * @brief MenuBar class for horizontal menu bars
 * Designed to be reusable with the same menu system as context menus
 */
class MenuBar : public Widget {
public:
    /**
     * @brief Create a menu bar
     * @return Created MenuBar instance or nullptr on failure
     */
    static MenuBar* create();

    MenuBar();
    ~MenuBar() override;

    bool init() override;

    /**
     * @brief Add a menu to the menu bar
     * @param title The title to display for this menu
     * @param menu The menu to add
     */
    void addMenu(std::string_view title, Menu* menu);

    /**
     * @brief Remove a menu from the menu bar
     * @param title The title of the menu to remove
     */
    void removeMenu(std::string_view title);

    /**
     * @brief Remove all menus
     */
    void removeAllMenus();

    /**
     * @brief Get the number of menus
     * @return Number of menus in this menu bar
     */
    size_t getMenuCount() const { return m_menus.size(); }

    /**
     * @brief Set the height of the menu bar
     * @param height The height in pixels
     */
    void setMenuBarHeight(float height);

    /**
     * @brief Get the height of the menu bar
     * @return The height in pixels
     */
    float getMenuBarHeight() const { return m_menuBarHeight; }

protected:
    void initRenderer() override;
    void updateLayout();
    void onMouseClicked() override;

private:
    struct MenuEntry {
        std::string title;
        Menu* menu;
        Widget* titleWidget;
        bool isOpen;
        
        MenuEntry(std::string_view t, Menu* m) 
            : title(t), menu(m), titleWidget(nullptr), isOpen(false) {}
    };

private:
    std::vector<MenuEntry> m_menus;
    float m_menuBarHeight;
    int m_activeMenuIndex;
    DrawShape* m_pBackground;
    
    static constexpr float MENU_BAR_DEFAULT_HEIGHT = 30.0f;
    static constexpr float MENU_TITLE_PADDING = 12.0f;
    static constexpr float MENU_TITLE_HEIGHT = 24.0f;
};

} // namespace ui

NS_OCF_END