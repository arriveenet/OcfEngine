#pragma once
#include "base/Event.h"

NS_OCF_BEGIN

class EventDispatcher : public GameObject {
public:
	EventDispatcher();
	~EventDispatcher();

	void dispatchEvent();
};

NS_OCF_END