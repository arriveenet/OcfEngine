#include "UIWidget.h"

OCF_BEGIN

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

void Widget::initRenderer()
{
}

}

OCF_END