#pragma once
#include"base/Macros.h"

namespace ocf {

class Node2D;

class Component {
public:
    Component(Node2D* pEntity, int updateOrder = 100);
    virtual ~Component();

    virtual void update(float deltaTime);
    int getUpdateOrder() const;

protected:
    Node2D* m_pOwner;
    int m_updateOrder;
};

} // namespace ocf
