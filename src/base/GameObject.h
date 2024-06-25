#pragma once
#include "base/Macros.h"
#include "base/Config.h"

OCF_BEGIN

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	unsigned int getID() const;

	void release();

private:
	unsigned int m_id;
};

OCF_END
