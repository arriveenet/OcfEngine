#pragma once
#include <string>
#include <vector>
#include "2d/Entity.h"
#include "base/types.h"
#include "base/Macros.h"
#include "renderer/VertexArray.h"

OCF_BEGIN

class Texture2D;

class Label : public Entity {
public:
	static Label* create(const std::string& text);

	Label();
	virtual ~Label();

	bool init();

	void setString(const std::string& text);
	void setTextColor(const glm::vec3& textColor);
	void setTextColor(unsigned char r, unsigned char g, unsigned b);
	const glm::vec3& getTextColor() const { return m_textColor; }

	void update(float deltaTime) override;
	void draw();

protected:
	void updateQuads();
	void updateVertex();

protected:
	std::string m_text;
	glm::vec3 m_textColor;
	bool m_isDirty;
	Texture2D* m_texture = nullptr;
	VertexArray m_vertexArray;
	std::vector<QuadV3fC3fT2f> m_quads;
	std::vector<unsigned short> m_indices;
};

OCF_END
