#pragma once
#include "renderer/CustomCommand.h"

NS_OCF_BEGIN

class MeshCommand : public CustomCommand {
public:
    MeshCommand();
    virtual ~MeshCommand();

    void init(float globalZOrder);

    void init(float globalZOrder, const glm::mat4& transform);
};

NS_OCF_END
