#pragma once
#include "UIButtonBase.h"

NS_OCF_BEGIN

namespace ui {

class CheckBox : public ButtonBase {
public:
    static CheckBox* create(bool value);

    CheckBox();
    ~CheckBox() override;

    bool isSelected() const { return m_isSelected; }
    void setSelected(bool value) { m_isSelected = value; }

    void setSize(const glm::vec2& size) override;
    void setSize(float width, float height) override;

protected:
    bool init() override;
    void initRenderer() override;

    void onMouseClicked() override;

    void resizeContent(float width, float height);

private:
    bool m_isSelected;
    DrawShape* m_pCheckMark;
};

} // namespace ui

NS_OCF_END