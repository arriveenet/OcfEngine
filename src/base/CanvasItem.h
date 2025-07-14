#pragma once
#include "base/Node.h"

NS_OCF_BEGIN

class CanvasItem : public Node {
public:
    CanvasItem();
    ~CanvasItem();

    void setVisible(bool visible);
    bool isVisible() const;

protected:
    bool m_visible; //!< 表示フラグ
};

NS_OCF_END
