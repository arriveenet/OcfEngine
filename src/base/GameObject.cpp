#include "base/GameObject.h"
#include <iostream>

NS_OCF_BEGIN

GameObject::GameObject()
    : m_referenceCount(1)
{
    static unsigned int objectCount = 0;

    m_id = ++objectCount;

    //std::cout << "GameObject: " << m_id << std::endl;
}

GameObject::~GameObject()
{
}

unsigned int GameObject::getID() const
{
    return m_id;
}

void GameObject::retain()
{
    m_referenceCount++;
}

void GameObject::release()
{
    m_referenceCount--;

    if (m_referenceCount == 0) {
        delete this;
    }
}

GameObject* GameObject::autorelease()
{
    //@Todo オートリリース機能を追加
    return this;
}

unsigned int GameObject::getReferenceCount()
{
    return m_referenceCount;
}

NS_OCF_END
