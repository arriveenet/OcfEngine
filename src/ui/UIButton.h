#pragma once
#include <functional>
#include "ui/UIWidget.h"

OCF_BEGIN

class Sprite;

namespace ui {

class Button : public Widget {
public:
	static Button* create();

	Button();
	virtual ~Button();

	bool init() override;
	void processInput(const InputState& inputState) override;
	void setOnClickCallback(std::function<void()> onClick);

protected:
	void initRenderer() override;

protected:
	Sprite* m_pButtonNormalRenderer;
	std::function<void()> m_onClick;
};

}

OCF_END
