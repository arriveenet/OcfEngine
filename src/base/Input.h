#pragma once
#include "types.h"
#include "Keyboard.h"

struct InputState {
	Keyboard keyboard;
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