#pragma once
#include "TrianglesCommand.h"

NS_OCF_BEGIN

class QuadCommand : public TrianglesCommand {
public:
    QuadCommand();
    ~QuadCommand();

    void init(float globalZOrder, Texture2D* texture, QuadV3fC3fT2f* pQuads, unsigned short* pIndices, size_t quadCount, const glm::mat4& modelView);

};

NS_OCF_END
