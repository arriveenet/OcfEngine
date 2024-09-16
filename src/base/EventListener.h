#pragma once
#include <functional>
#include "base/GameObject.h"
#include "input/Keyboard.h"

NS_OCF_BEGIN

class Event;

class EventListener : public GameObject {
public:
	enum class Type {
		Unknown,
		Keyboard,
		Mouse
	};

	EventListener();
	virtual ~EventListener();

	bool init(Type type, const std::function<void(Event*)>& callback);

	Type m_type;
	std::function<void(Event*)> m_onEvent;
};


class EventListenerKeyboard : public EventListener {
public:
	static EventListenerKeyboard* create();

	bool init();

	std::function<void(Keyboard::KeyCode, Event*)> m_onKeyPressed;
};

NS_OCF_END 
