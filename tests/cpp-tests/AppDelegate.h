#pragma once
#include "platform/Application.h"

class AppDelegate : private ocf::Applicaiton {
public:
	AppDelegate();
	virtual ~AppDelegate();

	bool applicationDidFinishLaunching() override;
};

