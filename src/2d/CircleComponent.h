#pragma once
#include <glm/glm.hpp>
#include "2d/Component.h"

NS_OCF_BEGIN

class CircleComponent : public Component {
public:
    CircleComponent(Node* pEntity);

    void setRadius(float radius) { m_radius = radius; }
    float getRadius() const;
    glm::vec2 getCenter() const;

private:
    float m_radius;
};

bool intersectCircle(const CircleComponent& a, const CircleComponent& b);

NS_OCF_END