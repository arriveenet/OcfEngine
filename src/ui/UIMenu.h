#pragma once
#include "ui/UIWidget.h"
#include <vector>
#include <memory>

NS_OCF_BEGIN

class DrawShape;

namespace ui {

class MenuItem;

/**
 * @brief Menu class that contains menu items
 * Base class for context menus and menu bars
 */
class Menu : public Widget {
public:
    /**
     * @brief Create an empty menu
     * @return Created Menu instance or nullptr on failure
     */
    static Menu* create();

    Menu();
    ~Menu() override;

    bool init() override;

    /**
     * @brief Add a menu item to this menu
     * @param item The menu item to add
     */
    void addMenuItem(MenuItem* item);

    /**
     * @brief Remove a menu item from this menu
     * @param item The menu item to remove
     */
    void removeMenuItem(MenuItem* item);

    /**
     * @brief Remove all menu items
     */
    void removeAllMenuItems();

    /**
     * @brief Get the number of menu items
     * @return Number of menu items in this menu
     */
    size_t getMenuItemCount() const { return m_menuItems.size(); }

    /**
     * @brief Get menu item at index
     * @param index The index of the menu item
     * @return Menu item at index or nullptr if index is out of bounds
     */
    MenuItem* getMenuItem(size_t index) const;

    /**
     * @brief Set menu visibility
     * @param visible True to show menu, false to hide
     */
    void setVisible(bool visible) override;

    /**
     * @brief Get whether menu is currently visible
     * @return True if visible, false otherwise
     */
    bool isVisible() const override;

protected:
    void initRenderer() override;
    void updateLayout();

protected:
    std::vector<MenuItem*> m_menuItems;
    bool m_isVisible;
    DrawShape* m_pBackground;
    
    static constexpr float MENU_DEFAULT_WIDTH = 140.0f;
    static constexpr float MENU_ITEM_HEIGHT = 24.0f;
    static constexpr float MENU_PADDING = 4.0f;
};

} // namespace ui

NS_OCF_END