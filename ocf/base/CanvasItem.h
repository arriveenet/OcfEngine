#pragma once
#include "base/Node.h"

namespace ocf {

class CanvasItem : public Node {
public:
    CanvasItem();
    ~CanvasItem();

    void setVisible(bool visible);
    bool isVisible() const;

protected:
    bool m_visible = true;  // 表示フラグ
};

} // namespace ocf
