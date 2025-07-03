#pragma once
#include "ui/UIButtonBase.h"

NS_OCF_BEGIN

namespace ui {

/**
 * @brief MenuItem class for menu entries
 * Extends ButtonBase to inherit action event functionality
 */
class MenuItem : public ButtonBase {
public:
    /**
     * @brief Create a menu item with text
     * @param text The text to display on the menu item
     * @return Created MenuItem instance or nullptr on failure
     */
    static MenuItem* create(std::string_view text = "");

    MenuItem();
    ~MenuItem() override;

    bool init() override;

    /**
     * @brief Set whether this menu item is enabled
     * @param enabled True to enable, false to disable
     */
    void setEnabled(bool enabled);
    
    /**
     * @brief Get whether this menu item is enabled
     * @return True if enabled, false if disabled
     */
    bool isEnabled() const { return m_enabled; }

    /**
     * @brief Set whether this menu item shows a separator line below it
     * @param separator True to show separator, false otherwise
     */
    void setSeparator(bool separator);
    
    /**
     * @brief Get whether this menu item shows a separator
     * @return True if separator is shown, false otherwise
     */
    bool isSeparator() const { return m_separator; }

protected:
    void initRenderer() override;
    void updateTextLocation() override;
    void onMouseClicked() override;
    void onMousePressed() override;
    void onMouseReleased() override;

private:
    bool m_enabled;
    bool m_separator;
    bool m_highlighted;
    
    static constexpr float MENU_ITEM_DEFAULT_WIDTH = 120.0f;
    static constexpr float MENU_ITEM_DEFAULT_HEIGHT = 24.0f;
    static constexpr float MENU_ITEM_TEXT_PADDING = 8.0f;
};

} // namespace ui

NS_OCF_END