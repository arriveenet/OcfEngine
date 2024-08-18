#pragma once
#include "base/Macros.h"

NS_OCF_BEGIN

class ApplicationBase {
public:
    virtual ~ApplicationBase();

    virtual bool applicationDidFinishLaunching() = 0;
};

NS_OCF_END
