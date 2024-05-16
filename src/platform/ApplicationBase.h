#pragma once
#include "base/Macros.h"

OCF_BEGIN

class ApplicationBase {
public:
	virtual ~ApplicationBase();

	virtual bool applicationDidFinishLaunching() = 0;
};

OCF_END
