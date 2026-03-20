#pragma once
#include <string>
#include "ocf/core/Macros.h"

namespace ocf {

namespace StringUtils {

std::u32string convertUtf8ToUtf32(std::string_view utf8String);

};

} // namespace ocf
