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
	initRenderer();
	return true;
}

void Widget::initRenderer()
{
}

}

OCF_END