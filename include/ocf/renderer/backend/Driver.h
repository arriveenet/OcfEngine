#pragma once
#include "renderer/backend/Handle.h"
#include "renderer/backend/DriverEnums.h"

namespace ocf::backend {

class Driver {
public:
    virtual ~Driver() = default;

    virtual TextureHandle createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                        uint32_t width, uint32_t height, uint32_t depth) = 0;
};

} // namespace ocf::backend