#pragma once
#include "ocf/2d/Component.h"
#include <glm/glm.hpp>

namespace ocf {

class CircleComponent : public Component {
public:
    CircleComponent(Node2D* pEntity);

    void setRadius(float radius) { m_radius = radius; }
    float getRadius() const;
    glm::vec2 getCenter() const;

private:
    float m_radius;
};

bool intersectCircle(const CircleComponent& a, const CircleComponent& b);

} // namespace ocf