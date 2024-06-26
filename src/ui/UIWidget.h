#pragma once
#include "2d/Node.h"

OCF_BEGIN

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

OCF_END