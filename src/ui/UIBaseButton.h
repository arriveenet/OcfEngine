#include "ui/UIWidget.h"
#include <functional>

NS_OCF_BEGIN

class Camera;

namespace ui {

class BaseButton : public Widget {
public:
    BaseButton();
    ~BaseButton() override;

 protected:
    void updateState();

    bool m_isHit = false;
    std::function<void()> m_onClick;
};

} // namespace ui

NS_OCF_END