#pragma once
#include "UIButtonBase.h"

NS_OCF_BEGIN

namespace ui {

class RadioButton : public ButtonBase {
public:
    static RadioButton* create(std::string_view text);

    RadioButton();
    ~RadioButton() override;

protected:
    bool init() override;
    void initRenderer() override;
    void updateTextLocation() override;
    void onMouseClicked() override;
};

} // namespace ui

NS_OCF_END
