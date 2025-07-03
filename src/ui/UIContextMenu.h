#pragma once
#include "ui/UIMenu.h"

NS_OCF_BEGIN

namespace ui {

/**
 * @brief ContextMenu class for popup menus
 * Appears at cursor position on right-click
 */
class ContextMenu : public Menu {
public:
    /**
     * @brief Create a context menu
     * @return Created ContextMenu instance or nullptr on failure
     */
    static ContextMenu* create();

    ContextMenu();
    ~ContextMenu() override;

    bool init() override;

    /**
     * @brief Show the context menu at specified position
     * @param position World position where to show the menu
     */
    void showAt(const glm::vec2& position);

    /**
     * @brief Hide the context menu
     */
    void hide();

    /**
     * @brief Check if context menu should auto-hide when clicking outside
     * @return True if auto-hide is enabled
     */
    bool isAutoHide() const { return m_autoHide; }

    /**
     * @brief Set whether context menu should auto-hide when clicking outside
     * @param autoHide True to enable auto-hide, false to disable
     */
    void setAutoHide(bool autoHide) { m_autoHide = autoHide; }

    /**
     * @brief Update method called every frame
     * @param deltaTime Time since last update
     */
    void updateNode(float deltaTime) override;

protected:
    void initRenderer() override;

private:
    /**
     * @brief Adjust menu position to stay within screen bounds
     * @param position Desired position
     * @return Adjusted position
     */
    glm::vec2 adjustPositionForBounds(const glm::vec2& position);

private:
    bool m_autoHide;
    glm::vec2 m_showPosition;
    
    static constexpr float CONTEXT_MENU_MARGIN = 4.0f;
};

} // namespace ui

NS_OCF_END