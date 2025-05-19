#pragma once
#include "ui/UIWidget.h"
#include <functional>

NS_OCF_BEGIN

class DrawShape;

namespace ui {

class ButtonBase : public Widget {
public:
    using ActionEvent = std::function<void()>;

    ButtonBase();
    ~ButtonBase() override;

    void updateNode(float deltaTime) override;

    void setOnAction(ActionEvent value);
    ActionEvent getOnAction() const { return m_onAction; }

 protected:
    DrawShape* m_pButtonBackground;
    ActionEvent m_onAction;
};

} // namespace ui

NS_OCF_END