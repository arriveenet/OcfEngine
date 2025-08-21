#pragma once
#include "2d/DrawShape.h"
#include "base/Rect.h"
#include "ui/UIWidget.h"
#include <functional>

namespace ocf {

namespace ui {

class Slider : public Widget {
public:
    static constexpr float DEFAULT_WIDTH = 200.0f;
    static constexpr float DEFAULT_HEIGHT = 10.0f;

    static Slider* create(float width = DEFAULT_WIDTH, float height = DEFAULT_HEIGHT);

    Slider();
    virtual ~Slider();

    bool init() override;
    void updateNode(float deltaTime) override;

    void setSize(const glm::vec2& size) override;

    void setValue(int value);
    void setMinValue(int minValue) { m_minValue = minValue; }
    void setMaxValue(int maxValue) { m_maxValue = maxValue; }

    int getValue() const { return m_value; }
    int getMinValue() const { return m_minValue; }
    int getMaxValue() const { return m_maxValue; }

    float getPercent() const;

    void setOnValueChangedCallback(std::function<void(int)> onValueChanged);

protected:
    void initRenderer() override;
    void updateSlider();

private:
    DrawShape* m_pSliderBackground;
    bool m_isDirty;
    bool m_isDragging;
    Rect m_thumbRect;
    glm::vec2 m_lastMousePosition;
    std::function<void(int)> m_onValueChanged;

    int m_value = 50;
    int m_minValue = 0;
    int m_maxValue = 100;
};

} // namespace ui

} // namespace ocf
