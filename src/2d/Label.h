#pragma once
#include <string>
#include <vector>
#include "2d/Entity.h"
#include "base/types.h"

class Label : public Entity {
public:
	static Label* create(const std::string& text);

	Label();
	virtual ~Label();

	void setString(const std::string& text);

	void update(float deltaTime) override;
	void draw();

protected:
	void updateQuads();

protected:
	std::string m_text;
	std::vector<QuadV3fT2f> m_quads;
	std::vector<unsigned int> m_indices;
};