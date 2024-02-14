#pragma once

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	unsigned int getID();

private:
	unsigned int m_id;
};