#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	static unsigned int objectCount = 0;

	m_id = ++objectCount;

	std::cout << "GameObject: " << m_id << std::endl;
}

GameObject::~GameObject()
{
}

unsigned int GameObject::getID()
{
	return m_id;
}
