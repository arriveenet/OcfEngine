#pragma once
#include "UIButtonBase.h"

NS_OCF_BEGIN

namespace ui {

class CheckBox : public ButtonBase {
public:
    static CheckBox* create();
    static CheckBox* create(std::string_view text);

    CheckBox();
    ~CheckBox() override;

    void setSize(const glm::vec2& size) override;
    void setSize(float width, float height) override;

protected:
    void initRenderer() override;
    void onMouseClicked() override;
    void resizeContent(float width, float height);
    void updateTextLocation() override;
};

} // namespace ui

NS_OCF_END