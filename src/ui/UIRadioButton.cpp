#include "UIRadioButton.h"

NS_OCF_BEGIN

namespace ui {

RadioButton* RadioButton::create()
{
    RadioButton* radioButton = new RadioButton();
    if (radioButton) {
        return radioButton;
    }
    OCF_SAFE_DELETE(radioButton);
    return nullptr;
}

RadioButton::RadioButton()
{
}

RadioButton::~RadioButton()
{
}

bool RadioButton::init()
{

}
void RadioButton::initRenderer()
{

}
}; // namespace ui

NS_OCF_END
