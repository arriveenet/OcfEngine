#pragma once
#include "renderer/CustomCommand.h"

namespace ocf {

class MeshCommand : public CustomCommand {
public:
    MeshCommand();
    virtual ~MeshCommand();

    void init(float globalZOrder);

    void init(float globalZOrder, const glm::mat4& transform);
};

} // namespace ocf
