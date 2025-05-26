#pragma once
#include "UIButtonBase.h"

NS_OCF_BEGIN

namespace ui {

class RadioButton : public ButtonBase {
public:
    static RadioButton* create();

    RadioButton();
    ~RadioButton() override;

protected:
    bool init() override;
    void initRenderer() override;
};

} // namespace ui

NS_OCF_END
