#pragma once
#include <string>
#include "base/Macros.h"

NS_OCF_BEGIN

namespace StringUtils {

std::u32string convertUtf8ToUtf32(std::string_view utf8String);

};

NS_OCF_END
