#pragma once
#include"base/Macros.h"

OCF_BEGIN

class Entity;

class Component {
public:
	Component(Entity* pEntity, int updateOrder = 100);
	virtual ~Component();

	virtual void update(float deltaTime);
	int getUpdateOrder() const;

protected:
	Entity* m_pOwner;
	int m_updateOrder;
};

OCF_END
