#pragma once
#include <string>
#include <functional>
#include "ui/UIWidget.h"

OCF_BEGIN

class Sprite;
class Label;

namespace ui {

class Button : public Widget {
public:
	static Button* create();

	Button();
	virtual ~Button();

	bool init() override;
	void processInput(const InputState& inputState) override;
	void setOnClickCallback(std::function<void()> onClick);
	void setText(const std::string& text);
	std::string getText() const;

protected:
	void initRenderer() override;
	bool createTextRendererIfNull();

protected:
	Sprite* m_pButtonNormalRenderer;
	Label* m_pTextLabel;
	std::function<void()> m_onClick;
};

}

OCF_END
