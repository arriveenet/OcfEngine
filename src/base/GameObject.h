#pragma once
#include "base/Macros.h"

NS_OCF_BEGIN

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	unsigned int getID() const;

	void release();

private:
	unsigned int m_id;
};

NS_OCF_END
