#pragma once
#include "Component.h"

namespace ocf {

class Node2D;

class MoveComponent : public Component {
public:
    MoveComponent(Node2D* pEntity, int updateOrder = 100);

    void update(float deltaTime) override;

    void setAngularSpeed(float speed) { m_angularSpeed = speed; }
    void setForwardSpeed(float speed) { m_forwardSpeed = speed; }
    float getAngularSpeed() const { return m_angularSpeed; }
    float getForwardSpeed() const { return m_forwardSpeed; }

private:
    float m_angularSpeed;
    float m_forwardSpeed;
};

} // namespace ocf
