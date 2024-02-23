#pragma once
#include <string>
#include <vector>
#include "2d/Entity.h"
#include "base/types.h"
#include "base/Macros.h"
#include "renderer/Texture2D.h"

OCF_BEGIN

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

	bool m_isUpdate;

	Texture2D* m_texture = nullptr;

	std::vector<QuadV3fT2f> m_quads;
	std::vector<unsigned short> m_indices;
};

OCF_END
