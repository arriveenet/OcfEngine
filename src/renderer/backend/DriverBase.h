#pragma once

#include "renderer/backend/Driver.h"
#include <stdint.h>

struct HwBase {
};

struct HwTexture : public HwBase {
    uint32_t width;
    uint32_t height;
};

namespace ocf::backend {

class DriverBase : public Driver {
public:
    DriverBase();
    ~DriverBase() override;
};

} // namespace ocf::backend