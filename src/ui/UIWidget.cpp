#include "UIWidget.h"
#include "UIContextMenu.h"

NS_OCF_BEGIN

namespace ui {

Widget::Widget()
{
}

Widget::~Widget()
{
}

bool Widget::init()
{
    if (Node::init()) {
        initRenderer();
        setAnchorPoint(glm::vec2(0.5f, 0.5f));

        return true;
    }
    return false;
}

void Widget::onMouseClicked() {}

void Widget::onMousePressed() {}

void Widget::onMouseReleased() {}

void Widget::onRightClicked() 
{
    // Show context menu if one is set
    if (m_contextMenu) {
        // Get mouse position (this would need to be passed from the event system)
        // For now, we'll show it at the widget's position
        const glm::vec2 position = getPosition();
        m_contextMenu->showAt(position);
    }
}

void Widget::onSetFocus() {}

void Widget::onKillFocus() {}

void Widget::setContextMenu(ContextMenu* contextMenu)
{
    m_contextMenu = contextMenu;
}

void Widget::initRenderer() {}

}

NS_OCF_END