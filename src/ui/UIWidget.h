#pragma once
#include "2d/Node.h"

NS_OCF_BEGIN

namespace ui {

class ContextMenu;

class Widget : public Node {
public:
    Widget();
    virtual ~Widget();

    virtual bool init();

    virtual void onMouseClicked();
    virtual void onMousePressed();
    virtual void onMouseReleased();
    virtual void onRightClicked(); // New method for right-click events
    virtual void onSetFocus();
    virtual void onKillFocus();

    bool isFocused() const { return m_focus; }

    /**
     * @brief Set the context menu for this widget
     * @param contextMenu The context menu to show on right-click
     */
    void setContextMenu(ContextMenu* contextMenu);

    /**
     * @brief Get the current context menu
     * @return The context menu or nullptr if none is set
     */
    ContextMenu* getContextMenu() const { return m_contextMenu; }

protected:
    virtual void initRenderer();

    bool m_focus = false;
    bool m_pressed = false;
    ContextMenu* m_contextMenu = nullptr;
};

}

NS_OCF_END