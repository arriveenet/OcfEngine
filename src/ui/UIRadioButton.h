#pragma once
#include "UIButtonBase.h"
#include <memory>

NS_OCF_BEGIN

namespace ui {

class ToggleGroup;

class Toggle {
public:
    virtual ~Toggle() = default;
    virtual void setToggleGroup(std::shared_ptr<ToggleGroup> group) = 0;
    virtual std::shared_ptr<ToggleGroup> getToggleGroup() const = 0;
    virtual bool isSelected() const = 0;
    virtual void setSelected(bool selected) = 0;
};


class ToggleGroup {
public:
    void addToggle(Toggle* toggle);
    void selectedToggle(Toggle* value);
    Toggle* getSelectedToggle() const;

private:
    std::vector<Toggle*> m_toggles;
};

class RadioButton : public ButtonBase, Toggle {
public:
    static RadioButton* create(std::string_view text);

    RadioButton();
    ~RadioButton() override;

    void setToggleGroup(std::shared_ptr<ToggleGroup> group) override;
    std::shared_ptr<ToggleGroup> getToggleGroup() const override;
    bool isSelected() const override;
    void setSelected(bool selected) override;

protected:
    bool init() override;
    void initRenderer() override;
    void updateTextLocation() override;
    void onMouseClicked() override;

private:
    std::shared_ptr<ToggleGroup> m_toggleGroup;
    bool m_isSelected;
};

} // namespace ui

NS_OCF_END
