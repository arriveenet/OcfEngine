#pragma once
#include "Component.h"

OCF_BEGIN

class Entity;

class MoveComponent : public Component {
public:
	MoveComponent(Entity* pEntity, int updateOrder = 100);

	void update(float deltaTime) override;

	void setAngularSpeed(float speed) { m_angularSpeed = speed; }
	void setForwardSpeed(float speed) { m_forwardSpeed = speed; }
	float getAngularSpeed() const { return m_angularSpeed; }
	float getForwardSpeed() const { return m_forwardSpeed; }

private:
	float m_angularSpeed;
	float m_forwardSpeed;
};

OCF_END
