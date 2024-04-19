#pragma once
#include "base/types.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"

OCF_BEGIN

struct InputState {
	Keyboard keyboard;
	Mouse mouse;
};

class Input {
public:
	bool init();

	void prepareUpdate();
	void update();

	const InputState& getState() const { return m_inputState; }

private:
	InputState m_inputState;
};

OCF_END
