#pragma once
#include "UIButtonBase.h"

namespace ocf {

namespace ui {

class CheckBox : public ButtonBase {
public:
    static CheckBox* create();
    static CheckBox* create(std::string_view text);

    CheckBox();
    ~CheckBox() override;

    void setSelected(bool selected);
    bool isSelected() const { return m_isSelected; }
    void setIndeterminate(bool indeterminate);
    bool isIndeterminate() const { return m_isIndeterminate; }

protected:
    void initRenderer() override;
    void onMouseClicked() override;
    void setCheckBoxSize(float width, float height);
    void updateTextLocation() override;

    bool m_isSelected;
    bool m_isIndeterminate;
    glm::vec2 m_checkBoxSize;
};

} // namespace ui

} // namespace ocf