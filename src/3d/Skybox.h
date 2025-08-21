#pragma once
#include "base/Node.h"
#include "renderer/TextureCube.h"
#include "renderer/CustomCommand.h"
#include <string>

namespace ocf {

class Skybox : public Node {
public:
    static Skybox* create(std::string_view positive_x,
                          std::string_view negative_x,
                          std::string_view positive_y,
                          std::string_view negative_y,
                          std::string_view positive_z,
                          std::string_view negative_z);

    Skybox();
    virtual ~Skybox();

    bool init() override;
    bool init(std::string_view positive_x,
              std::string_view negative_x,
              std::string_view positive_y,
              std::string_view negative_y,
              std::string_view positive_z,
              std::string_view negative_z);

    void initBuffer();

    void draw(Renderer* renderer, const glm::mat4& transform) override;

protected:
    TextureCube* m_textureCube;
    CustomCommand m_customCommand;
};

} // namespace ocf
