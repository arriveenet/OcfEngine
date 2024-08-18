#pragma once
#include <string>
#include <functional>
#include "ui/UIWidget.h"

NS_OCF_BEGIN

class Sprite;
class Label;
class DrawShape;

namespace ui {

class Button : public Widget {
public:
    static Button* create();
    static Button* create(std::string_view normalImage, std::string_view activeImage);

    Button();
    virtual ~Button();

    bool init() override;
    bool init(std::string_view normalImage, std::string_view activeImage);
    void processInput(const InputState& inputState) override;
    void setOnClickCallback(std::function<void()> onClick);
    void setText(const std::string& text);
    std::string getText() const;

protected:
    void initRenderer() override;
    bool createTextRendererIfNull();
    void updateTextLocation();

protected:
    Sprite* m_pButtonNormalRenderer;
    Sprite* m_pButtonActiveRenderer;
    std::string m_normalFilename;
    std::string m_activeFilename;
    Label* m_pTextRenderer;
    std::function<void()> m_onClick;
};

}

NS_OCF_END
