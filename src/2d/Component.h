#pragma once
#include"base/Macros.h"

OCF_BEGIN

class Node;

class Component {
public:
	Component(Node* pEntity, int updateOrder = 100);
	virtual ~Component();

	virtual void update(float deltaTime);
	int getUpdateOrder() const;

protected:
	Node* m_pOwner;
	int m_updateOrder;
};

OCF_END
