#pragma once

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	unsigned int getID() const;

	void release();

private:
	unsigned int m_id;
};