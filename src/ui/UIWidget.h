#pragma once
#include "2d/Node.h"

NS_OCF_BEGIN

namespace ui {

class Widget : public Node {
public:
    Widget();
    virtual ~Widget();

    virtual bool init();

protected:
    virtual void initRenderer();
};

}

NS_OCF_END