#pragma once
#include "TrianglesCommand.h"

namespace ocf {

class QuadCommand : public TrianglesCommand {
public:
    QuadCommand();
    ~QuadCommand();

    void init(float globalZOrder, Texture2D* texture, QuadV3fC3fT2f* pQuads, unsigned short* pIndices, size_t quadCount, const glm::mat4& modelView);

};

} // namespace ocf
